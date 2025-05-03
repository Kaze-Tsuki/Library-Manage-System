# Library Managing System

<br>
<div>
    <img alt="C++" src="https://img.shields.io/badge/C++-17-%2300599C?logo=cplusplus">
</div>
<div>
    <img alt="platform" src="https://img.shields.io/badge/platform-Windows%20%7C%20Linux%20%7C%20macOS-blueviolet">
</div>
<div>
    <img alt="commit" src="https://img.shields.io/github/commit-activity/m/Kaze-Tsuki/Libarary2.6.2?color=%23ff69b4">
</div>
<div>
    <a href="https://github.com/Kaze-Tsuki/Libarary2.6.2">
    <img alt="Static Badge" src="https://img.shields.io/badge/GitHub-Link-%23B766AD"></a>

</div>
<br>

## Catolog
- [Library Managing System](#library-managing-system)
  - [Catolog](#catolog)
  - [Description](#description)
  - [Installation](#installation)
  - [Usage](#usage)
    - [Interface](#interface)
    - [Features](#features)
  - [Bugs](#bugs) 

## Description
This is a library management system that allows users to manage books, authors, and etc. It provides a user-friendly GUI for easy navigation and management of library resources. It also supports multi threading to be close to real world application. The system is designed to be easy to use and efficient, making it a great choice for libraries of all sizes.

## Installation
To install the library management system, follow these steps:
*This is a Windows only project, and it is not tested on other platforms.*
1. Clone the repository to your local machine using the following command:
   ```bash
   git clone https://github.com/Kaze-Tsuki/Libarary2.6.2.git
   ```
2. Be sure that you have installed C++ extension **SMFL 2.6.2**, you can download it from [here](https://www.sfml-dev.org/download.php). And prepare a suitable environment for SMFL [Instructions](https://www.sfml-dev.org/tutorials/2.6/).

3. Open the project in your preferred IDE (e.g., Visual Studio, Code::Blocks, etc.).

4. Build the project to compile the source code.

5. Run the executable file to start the library management system.

6. Follow the on-screen instructions or read the following paragraph to navigate through the system and manage your library resources.

*If you are not using Windows, please disable the control-V feature in input.cpp and comment the `#include <windows.h>` line in input.cpp. And you may need to modify the code to make it work on your platform.*

## Usage

This part will introduce how to use the library management system. It will cover the interface and features of the system.

### Interface

1. Main Interface: Including 6 Links to manage the library resources.
   - **Add Book**: Add a new book to the library.
   - **View Book**: View the details of a book in the library.
   - **List Book**: List all the books in the library.
   - **List User**: List all the users in the library.
   - **Save**: Save the current state of the library to a file.
   - **Load**: Load the library state from a file.

2. Add Book: This interface allows you to add a new book to the library. You can enter the book's name, author, and other details.
    
    - **Name**: The name of the book.
    - **Author**: The author of the book.
    - **ISBN**: The ISBN number of the book.
    - **Category**: The category of the book (e.g., Fiction, Non-Fiction, etc.).
    - **Copy**: The number of copies of the book available in the library.
    - **Published Date**: The year the book was published.

3. View Book: This interface allows you to view the details of a book in the library. You can use `next` and `prev` to navigate through the books. You can also borrow or return a book from this interface. The books are arranged by added time.

4. List Book: This interface allows you to list all the books in the library. You can use `next` and `prev` to navigate through the books. You can use the `filter button` to filter the books by name, author, category, and published year sorted by the attribute you want in ascending or descending order. `On clicking the books`, you will be redirected to the view book interface of the book.
Each Line contains the following information:
    - **Name**
    - **Author**
    - **Category**
    - **Copy**: available copies / total copies
    - **Published Date**

5. List User: This interface allows you to list all the users in the library. You can use next and prev to navigate through the users. `On clicking the users`, you will be redirected to the view user interface of the user.
Each Line contains the following information:
    - **Name**
    - **Borrowed Books**: The number of books borrowed by the user.

6. View User: This interface allows you to view the books borrowed by a user. You can use `next` and `prev` to navigate through the books. You can also return a book **directly** from this interface. The books are arranged by borrowed time.
Each Line contains the following information:
    - **Name**
    - **Due Date**: The date when the book is due to be returned.

7. Borrow Book: This interface is triggered when you click the borrow button in the view book interface. It allows you to borrow a book from the library. You can enter the user's name and the due date for returning the book.
    - **User Name**
    - **Due Date**

8. Return Book: This interface is triggered when you click the return button in the view book interface. It allows you to return a book to the library. You can enter the user's name to return a book.
    - **User Name**

9. Save: This interface allows you to save the current state of the library to a file. You can enter the file name and the data will be stored as json format.
    - **File Name**

10. Load: This interface allows you to load the library state from a file. You can enter the file name and the data will be loaded from json format.
    - **File Name**

### Features

- **Multi-threading**: The system supports multi-threading to handle multiple users and books simultaneously. This makes it more efficient and closer to a real-world application.
- **User-friendly GUI**: The system provides a user-friendly GUI for easy navigation and management of library resources. The interface is designed to be intuitive and easy to use, making it accessible to users of all ages.
- **Book Management**: The system allows you to add, view, and list books in the library. You can also filter the books by name, author, category, and published year.
- **Intuitive Navigation**: The system provides intuitive navigation through the use of links between different interfaces. You can easily switch between different sections of the system to manage your library resources.
- **Json File Format**: The system uses JSON file format to store the library state. This makes it easy to read and write data, and allows for easy integration with other systems. Or develop a webpage to display the data.

## Bugs

For the project is not fully tested, there may be some bugs in the code. If you encounter any bugs, please report them in the issues section of the repository.
Following will list some possible bugs:

- Due to the multi-threading, if there are too many changes happen at the same time, the system may not link to the right place or even crash.

- The Save system may stored no name user causing the available books more than the total books.

- The Load system may not load the data correctly if the file is not in the correct format or if there are any errors in the data.