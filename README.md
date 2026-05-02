# Email Management System (EMS)

> Developed as a Data Structures & Algorithms semester project, with emphasis on implementing core functionality through custom-built data structures rather than GUI/database integration.. The system simulates core email functionality — composing, sending, organizing, filtering spam, and managing social connections — entirely using custom-built data structures without any external libraries.

---

## 📌 Table of Contents

- <a href="#overview">Overview</a>
- <a href="#features">Features</a>
- <a href="#data-structures-used">Data Structures Used</a>
- <a href="#algorithms--concepts">Algorithms & Concepts</a>
- <a href="#file-persistence">File Persistence</a>
- <a href="#ui--input">UI & Input</a>
- <a href="#project-structure">Project Structure</a>
- <a href="#how-to-compile--run">How to Compile & Run</a>
- <a href="#sample-data">Sample Data</a>
- <a href="#author-contact">Author & Contact</a>

---

## <h2 id="how-to-compile--run">🚀 How to Compile & Run</h2>

### Requirements
- g++ compiler (MinGW on Windows, GCC on Linux/Mac)
- C++11 or later

### Compile
```bash
g++ *.cpp -o main
````

### Run

```bash
./main       # Linux/Mac
main         # Windows
```

## <h2 id="sample-data">📂 Sample Data</h2>

The project includes **sample data files for demonstration and testing purposes**.

These files allow the system to run immediately after compilation without requiring manual setup.

Included sample files:

* `users.txt`
* `email.txt`
* `spam_words.txt`
* `social_graph.txt`

---

## <h2 id="project-structure">📁 Project Structure</h2>

```
EmailManagementSystem/
├──demo/
│   └──ems.mp4
├── main.cpp              # Entry point, all screen/menu functions
├── EM.h                  # Core EMS class — login, compose, admin, settings
├── User.h                # User class — folders, send, inbox, contacts
├── EF.h                  # EmailFolder class — folder operations
├── Email.h               # Email class — data model
├── Contact.h / .cpp      # Contact class
├── attachment.h / .cpp   # Attachment class
├── SocialGraph.h         # Graph-based social network
├── BinaryTree.h          # Generic BST (template)
├── Linkedlist.h          # Generic singly linked list (template)
├── DLL.h                 # Generic doubly linked list (template)
├── MaxHeap.h             # Generic max-heap (template)
├── validateInput.h / .cpp # Safe input functions
├── colors.h              # ANSI terminal color macros
├── UserEnvironment.h     # Helper for email creation
│
├── users.txt
├── email.txt
├── spam_words.txt
└── social_graph.txt
```

---

## ✨ Features

### 🔐 User Authentication

* **Create Account** — Register with a username, valid email, and strong password

  * Email is validated for correct format (must contain `@` and `.`)
  * Password must be at least 8 characters and contain uppercase, lowercase, digit, and special character
  * Duplicate email addresses are rejected
  * **User ID is auto-generated** by incrementing from the last registered user ID
* **Login** — Authenticate using email and password

  * Last login date is updated automatically on every successful login
* **Forgot Password** — Recover password by providing registered email

---

### ✉️ Email Composition

* Compose emails with:

  * Auto-generated Email ID (increments from last email in file)
  * Receiver email and name
  * Subject and content
* Four actions after composing:

  * **Send** — Delivers to sender's Sent folder and receiver's Inbox
  * **Save as Draft** — Saves to Drafts folder
  * **Schedule** — Queues the email using a `queue<Email>` with a user-specified date and time
  * **Add Attachment** — Attach files with name, type, and auto-generated random size

---

### 📂 Folder Management

Every user has these default folders:

* **Inbox** — Received emails
* **Sent** — Sent emails
* **Drafts** — Unsent saved emails
* **Spam** — Auto-detected spam emails
* **Trash** — Deleted emails (soft delete)
* **Important (Import)** — Starred emails
* **Backup** — Copy of deleted emails for recovery

Users can also **create custom folders**.

#### Inside each folder:

| Operation            | Description                                               |
| -------------------- | --------------------------------------------------------- |
| **View Emails**      | Display all emails in the folder                          |
| **Sort by Priority** | Heap-sorts emails by `(priority - spamScore)`             |
| **Sort by Date**     | Heap-sorts emails by timestamp                            |
| **Move Email**       | Move an email to any other folder                         |
| **Undo Move**        | Restore a moved email back to original folder             |
| **Mark Read/Unread** | Toggle read status, updates file                          |
| **Delete Email**     | Soft-deletes to Trash + backs up to Backup folder         |
| **Undo Delete**      | Restore deleted email from Backup back to original folder |
| **Get Spam Score**   | View spam score of a specific email                       |
| **Set Priority**     | Change the priority of an email                           |
| **Star Email**       | Mark as important and move to Important folder            |
| **Empty Trash**      | Permanently delete all emails in Trash                    |
| **Clear Backup**     | Wipe the Backup folder                                    |

---

### 🚫 Spam Detection

* A list of spam keywords is loaded from `spam_words.txt` on startup
* When an email is sent or received, its **content is scanned** for spam words
* For every spam word found, the email's `spamScore` is incremented by 1
* Any email with `spamScore >= 1` is automatically flagged as spam and copied to the Spam folder
* Spam score also **reduces effective priority**: emails are ranked by `priority - spamScore`

---

### 🔍 Search & Filter

* **Quick Search (BST)** — Search contacts by:

  * Name — traverses BST matching by name field
  * Email — traverses BST matching by email field
* **View Recent Contacts** — Displays the last 10 contacted users (maintained as a fixed-size array with shift-on-insert)

---

### 👤 Contact Management (Social Graph)

* **Add Connection** — Create a bidirectional connection between two users with a connection strength (1–10)
* **Remove Connection** — Remove a connection and update the graph file
* **View Friends** — List all connections of a user with connection strength, date, and spam probability
* **Mutual Connections** — Find users connected to both given users
* **Friend Suggestions** — Suggest second-level connections (friends of friends) not yet directly connected
* **Spam Probability** — Each connection has a spam probability calculated as `(10 - strength) × 10%`

---

### ⚙️ Settings

* **Spam Filter Settings** — Explains how spam scoring works
* **Priority Rules** — Explains how email priority ranking is calculated

---

### 🛡️ Admin Panel

Access system-wide statistics without logging in:

* **System Statistics:**

  * Total number of registered users
  * Total spam words loaded
  * System health status (Email System, Spam Filter, Authentication)
  * Total emails, spam emails, read/unread count, scheduled emails
  * Spam percentage across all emails
* **User Management:**

  * View total user count
  * View all users (BST in-order traversal)
* **Spam Analysis:**

  * Spam word list
  * Spam email count and percentage

---

## <h2 id="data-structures-used">🧱 Data Structures Used</h2>

| Data Structure               | Where Used            | Purpose                                                                   |
| ---------------------------- | --------------------- | ------------------------------------------------------------------------- |
| **BST (Binary Search Tree)** | `BinaryTree.h`        | Stores users (searchable by email/name), stores contacts per user         |
| **Singly Linked List**       | `Linkedlist.h`        | Stores the list of email folders per user                                 |
| **Doubly Linked List (DLL)** | `DLL.h`               | Stores emails inside each folder — supports forward/backward traversal    |
| **Max-Heap**                 | `MaxHeap.h`           | Two heaps per folder: one for priority sorting, one for timestamp sorting |
| **Stack**                    | `EF.h` (std::stack)   | Tracks deleted emails for undo functionality                              |
| **Queue**                    | `User.h` (std::queue) | Holds scheduled emails                                                    |
| **Hash Map (unordered_map)** | `SocialGraph.h`       | Adjacency list for the social graph                                       |
| **Hash Set (unordered_set)** | `SocialGraph.h`       | Used in mutual friends calculation                                        |
| **Array**                    | `User.h`, `EM.h`      | Fixed-size recent contacts array (last 10), spam words array              |
| **Vector**                   | Throughout            | Dynamic result sets for BST search, token parsing                         |

---

## <h2 id="algorithms--concepts">🔬 Algorithms & Concepts</h2>

| Concept                          | Where Applied                                                                |
| -------------------------------- | ---------------------------------------------------------------------------- |
| **BST Insertion & Search**       | User registration and lookup, contact search by name/email                   |
| **Heap Sort**                    | Sorting emails by priority or timestamp inside folders                       |
| **Heapify**                      | Rebuilding heap after sort to maintain heap property                         |
| **Graph (Undirected, Weighted)** | Social connections between users with strength as weight                     |
| **BFS-style Traversal**          | Friend suggestions (2nd-level connections)                                   |
| **Operator Overloading**         | `<`, `>`, `==` on Email, User, Contact for BST ordering                      |
| **Iterator Pattern**             | Custom iterators on LinkedList and DLL for range-based loops                 |
| **Recursive Input Validation**   | `getInteger()`, `getString()`, `getCharacter()` retry on bad input           |
| **File I/O**                     | All data persisted to `.txt` files; read on startup, updated on every change |
| **String Parsing**               | Manual comma-delimited tokenization for file reading (no external parser)    |

---

## <h2 id="file-persistence">💾 File Persistence</h2>

All data is saved to plain text files and reloaded every time the program starts:

| File               | Format                                                                  | When Updated                  |
| ------------------ | ----------------------------------------------------------------------- | ----------------------------- |
| `users.txt`        | `id,username,email,password,createdDate,lastLogin`                      | Account creation, login       |
| `email.txt`        | `id,sender,receiver,subject,content,date,isRead,isSpam,priority,folder` | Send, delete, move, mark read |
| `social_graph.txt` | `user1,user2,strength,date`                                             | Add/remove connection         |
| `spam_words.txt`   | `word1,word2,...`                                                       | Read-only at startup          |

---

## <h2 id="ui--input">🎨 UI & Input</h2>

* **Color-coded terminal output** using ANSI escape codes (defined in `colors.h`):

  * 🟡 Yellow — section headers
  * 🟢 Green — prompts
  * 🔴 Red — errors and warnings
  * 🔵 Cyan — welcome screens and main menu
  * 🔵 Blue — admin statistics
* **Safe input validation** via `validateInput.cpp`:

  * `getInteger()` — rejects non-numeric input
  * `getString()` — reads full line including spaces
  * `getCharacter()` — accepts only a single alphabetic character
  * `getFloat()` — validates decimal numbers
  * All functions **retry recursively** on invalid input

---

## <h2 id="author-contact">👤 Author & Contact</h2>

**Laiba Atiq**

* GitHub: [github](https://github.com/Laiba-Atiq)
* Email: [email](mailto:laibatiq@gmail.com)

---

