const valuesContainer = document.getElementById("values");
const logsTableBody = document.querySelector("#logs-table tbody");

const cardLabels = ["TDS", "Turbidity", "Water Level"];
const TDS_THRESHOLD = 500;
const TURBIDITY_THRESHOLD = 75;

function renderCards(entries) {
  valuesContainer.innerHTML = "";

  entries.forEach(([_, value], index) => {
    const card = document.createElement("article");
    card.className = "card";
    card.innerHTML = `
            <h2>${cardLabels[index] || `Value ${index + 1}`}</h2>
            <p>${value}</p>
        `;
    valuesContainer.appendChild(card);
  });
}

async function loadValues() {
  try {
    const response = await fetch("/data");
    const payload = await response.json();
    const data = payload.data || {};

    const entries = [
      ["TDS", data.TDS ?? data.tds ?? "-"],
      ["Turbidity", data.Turbidity ?? data.turbidity ?? "-"],
      ["WaterLevel", data.WaterLevel ?? data.water_level ?? "-"],
    ];

    renderCards(entries);
  } catch (error) {
    console.error("Failed to fetch data:", error);
  }
}

function renderLogs(rows) {
  logsTableBody.innerHTML = "";
  rows.forEach((r) => {
    const tr = document.createElement("tr");
    const tdsValue = Number.parseFloat(r.tds ?? r.TDS ?? "NaN");
    const turbidityValue = Number.parseFloat(
      r.turbidity ?? r.Turbidity ?? "NaN",
    );

    const isTdsHigh = Number.isFinite(tdsValue) && tdsValue > TDS_THRESHOLD;
    const isTurbidityHigh =
      Number.isFinite(turbidityValue) && turbidityValue > TURBIDITY_THRESHOLD;

    if (isTdsHigh) {
      tr.classList.add("row-high");
    } else if (isTurbidityHigh) {
      tr.classList.add("row-warning");
    } else {
      tr.classList.add("row-safe");
    }

    const date = document.createElement("td");
    date.textContent = r.date || "-";
    const time = document.createElement("td");
    time.textContent = r.time || "-";
    const td = document.createElement("td");
    td.textContent = r.tds || r.TDS || "-";
    const turb = document.createElement("td");
    turb.textContent = r.turbidity || r.Turbidity || "-";
    const wl = document.createElement("td");
    wl.textContent = r.water_level || r.WaterLevel || r.waterLevel || "-";

    tr.appendChild(date);
    tr.appendChild(time);
    tr.appendChild(td);
    tr.appendChild(turb);
    tr.appendChild(wl);
    logsTableBody.appendChild(tr);
  });
}

async function loadLogs() {
  try {
    const response = await fetch("/logs?limit=50");
    const payload = await response.json();
    const logs = payload.logs || [];
    renderLogs(logs);
  } catch (err) {
    console.error("Failed to load logs", err);
  }
}

loadValues();
loadLogs();
setInterval(loadValues, 1000);
setInterval(loadLogs, 3000);

const clearBtn = document.querySelector(".clear-logs-btn");
if (clearBtn) {
  clearBtn.addEventListener("click", async (e) => {
    e.preventDefault();
    if (!confirm("Clear all logs? This cannot be undone.")) return;
    try {
      const res = await fetch("/logs/clear", { method: "POST" });
      if (!res.ok) throw new Error("Failed to clear logs");
      await loadLogs();
      alert("Logs cleared");
    } catch (err) {
      console.error(err);
      alert("Failed to clear logs");
    }
  });
}
