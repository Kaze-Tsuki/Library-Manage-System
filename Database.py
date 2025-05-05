import requests
import json

def fetch_book_data(isbn_list):
    """
    Fetch book data from Open Library API using a list of ISBNs.
    """
    books = []
    for isbn in isbn_list:
        url = f"https://openlibrary.org/api/books?bibkeys=ISBN:{isbn}&format=json&jscmd=data"
        response = requests.get(url)
        if response.status_code == 200:
            data = response.json()
            book_key = f"ISBN:{isbn}"
            if book_key in data:
                book_info = data[book_key]
                # Parse publish_date into year, month, and day
                publish_date = book_info.get("publish_date", "Unknown Date")
                try:
                    # Attempt to split the date into components
                    date_parts = publish_date.split()
                    year = int(date_parts[-1]) if len(date_parts) >= 1 else 3000
                    month = int(date_parts[0]) if len(date_parts) >= 2 else 1
                    day = int(date_parts[1]) if len(date_parts) >= 3 else 1
                except ValueError:
                    # Fallback to default values if parsing fails
                    year, month, day = 3000, 1, 1

                book = {
                    "ISBN": isbn,
                    "name": book_info.get("title", "Unknown Title"),
                    "author": ", ".join([author["name"] for author in book_info.get("authors", [])]),
                    "published": [year, month, day],  # Use parsed year, month, and day
                    "category": book_info.get("subjects", [{"name": "Unknown"}])[0].get("name", "Unknown"),
                    "copyAmount": 1,
                    "availableCopies": 1,
                    "due": [2025, 1, 1]  # Default due date
                }
                books.append(book)
        else:
            print(f"Failed to fetch data for ISBN: {isbn}")
    
    return {"books": books}

def save_to_json(data, filename="books.json"):
    """
    Save the book data to a JSON file with ASCII encoding.
    """
    with open(filename, "w", encoding="utf-8") as f:
        json.dump(data, f, indent=4, ensure_ascii=True)  # Ensure ASCII encoding

if __name__ == "__main__":
    # Example ISBN list
    isbn_list = ["9780590353427", "9780142437148", "9780679761049", "9780679752702",
                 "9780811204811", "9780871401730", "9780375704024", "9781577315933",
                 "9780679775430", "9780679734505","9780143039341", "9781853267338",
                 "0307388816", "9780465021475", "9781328662057", "9781583335086",
                 "9781462524921", "9781509550258"]
    book_data = fetch_book_data(isbn_list)
    save_to_json(book_data)
    print("Book data saved to books.json")