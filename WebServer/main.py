import uvicorn
from fastapi import FastAPI, Request
from fastapi.responses import FileResponse
from fastapi.staticfiles import StaticFiles
from datetime import datetime
import csv
from pathlib import Path
from fastapi import Response

app = FastAPI()

BASE_DIR = Path(__file__).resolve().parent
STATIC_DIR = BASE_DIR / "static"
LOG_FILE = BASE_DIR / "logs.csv"
LOG_FIELDS = ["date", "time", "tds", "turbidity", "water_level"]
latest_data = {
    "TDS": "-",
    "Turbidity": "-",
    "WaterLevel": "-",
}

app.mount("/static", StaticFiles(directory=STATIC_DIR), name="static")


@app.get("/")
async def serve_index():
    return FileResponse(BASE_DIR / "index.html")

@app.post('/data')
async def receive_data(request: Request):
    data = await request.json()
    if isinstance(data, dict):
        latest_data["TDS"] = data.get("TDS", "-")
        latest_data["Turbidity"] = data.get("Turbidity", "-")
        latest_data["WaterLevel"] = data.get("WaterLevel", "-")
        log_entry = {
            "date": datetime.now().date(),
            "time": datetime.now().strftime("%H:%M"),
            "tds": data.get("TDS", "-"),
            "turbidity": data.get("Turbidity", "-"),
            "water_level": data.get("WaterLevel", "-"),
        }
        print(log_entry)

        write_header = not LOG_FILE.exists() or LOG_FILE.stat().st_size == 0
        with LOG_FILE.open("a", newline="", encoding="utf-8") as log_file:
            writer = csv.DictWriter(
                log_file,
                fieldnames=LOG_FIELDS,
            )
            if write_header:
                writer.writeheader()
            writer.writerow(log_entry)

    print(f"Data received: {data}")

    return {"status": "success", "received": data}


@app.get('/data')
async def get_data():
    return {"data": latest_data}


@app.get('/logs')
async def get_logs(limit: int = 100):
    rows = []
    if LOG_FILE.exists() and LOG_FILE.stat().st_size > 0:
        with LOG_FILE.open("r", newline="", encoding="utf-8") as log_file:
            reader = csv.DictReader(log_file)
            rows = list(reader)

    rows = list(reversed(rows))[: max(1, limit)]
    return {"logs": rows}


@app.get('/logs/download')
async def download_logs():
    if not LOG_FILE.exists():
        with LOG_FILE.open("w", newline="", encoding="utf-8") as log_file:
            writer = csv.DictWriter(log_file, fieldnames=LOG_FIELDS)
            writer.writeheader()

    return FileResponse(
        path=LOG_FILE,
        media_type="text/csv",
        filename="logs.csv",
    )


@app.post('/logs/clear')
async def clear_logs():
    """Truncate the logs file and write a fresh header."""
    # Ensure file exists with header only
    with LOG_FILE.open("w", newline="", encoding="utf-8") as log_file:
        writer = csv.DictWriter(log_file, fieldnames=LOG_FIELDS)
        writer.writeheader()

    return {"status": "cleared"}


if __name__ == "__main__":
    uvicorn.run(app=app, host = "0.0.0.0", port = 8000)
