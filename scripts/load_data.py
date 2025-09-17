from pathlib import Path

import requests

URL = [
    "https://pub-383410a98aef4cb686f0c7601eddd25f.r2.dev/graphalytics/wiki-Talk.tar.zst",
    "https://pub-383410a98aef4cb686f0c7601eddd25f.r2.dev/graphalytics/dota-league.tar.zst",
    "https://pub-383410a98aef4cb686f0c7601eddd25f.r2.dev/graphalytics/test-bfs-directed.tar.zst",
    "https://pub-383410a98aef4cb686f0c7601eddd25f.r2.dev/graphalytics/test-bfs-undirected.tar.zst",
    "https://pub-383410a98aef4cb686f0c7601eddd25f.r2.dev/graphalytics/test-cdlp-directed.tar.zst",
    "https://pub-383410a98aef4cb686f0c7601eddd25f.r2.dev/graphalytics/test-cdlp-undirected.tar.zst",
    "https://pub-383410a98aef4cb686f0c7601eddd25f.r2.dev/graphalytics/test-pr-directed.tar.zst",
    "https://pub-383410a98aef4cb686f0c7601eddd25f.r2.dev/graphalytics/test-pr-undirected.tar.zst",
    "https://pub-383410a98aef4cb686f0c7601eddd25f.r2.dev/graphalytics/test-lcc-directed.tar.zst",
    "https://pub-383410a98aef4cb686f0c7601eddd25f.r2.dev/graphalytics/test-lcc-undirected.tar.zst",
    "https://pub-383410a98aef4cb686f0c7601eddd25f.r2.dev/graphalytics/test-wcc-directed.tar.zst",
    "https://pub-383410a98aef4cb686f0c7601eddd25f.r2.dev/graphalytics/test-wcc-undirected.tar.zst",
    "https://pub-383410a98aef4cb686f0c7601eddd25f.r2.dev/graphalytics/test-sssp-directed.tar.zst",
    "https://pub-383410a98aef4cb686f0c7601eddd25f.r2.dev/graphalytics/test-sssp-undirected.tar.zst",
]


def make_dir() -> Path:
    benchmark_dir = Path(__file__).parent.parent / "benchmark"
    benchmark_dir.mkdir(parents=True, exist_ok=True)
    return benchmark_dir


def download_file(
    url: str, download_dir: Path, filename: str | None = None, overwrite: bool = False
) -> None:
    """
    Download a file from a URL to a specified directory.

    Args:
        url (str): The URL of the file to download
        download_dir (Path): The directory to save the downloaded file
        filename (str | None): The name to save the file as (optional, will extract from URL if not provided)
        overwrite (bool): Whether to overwrite the existed file (optional, will ignore by default)
    """
    # Extract filename from URL if not provided
    if filename is None:
        filename = url.split("/")[-1]

    # Create full file path
    file_path = download_dir / filename

    if file_path.exists() and file_path.is_file() and not overwrite:
        print(f"✅ File existed at: {file_path}")
        return

    try:
        # Download the file with streaming
        with requests.get(url, stream=True, timeout=60) as response:
            response.raise_for_status()  # Raise exception for bad status codes

            # Write file content in chunks
            with open(file_path, "wb") as file:
                for chunk in response.iter_content(chunk_size=8192):
                    if chunk:  # Filter out keep-alive chunks
                        file.write(chunk)

        print(f"✅ File downloaded successfully to: {file_path}")

    except requests.exceptions.RequestException as e:
        print(f"❌ Download {filename} failed:\n{e}")
        # Consider removing partially downloaded file here
        if file_path.exists():
            file_path.unlink()


def load_data(urls: list[str]):
    benchmark_dir = make_dir()
    for url in urls:
        download_file(url, benchmark_dir)


if __name__ == "__main__":
    load_data(URL)
