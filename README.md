# 🎬 Movie Ticket Booking System (C Language)

A simple **console-based ticket booking system** for managing movie screenings, seat bookings, cancellations, and revenue tracking. Built using **C**, this project demonstrates file handling, structured programming, and basic authentication logic.

---

## 🛠 Features

### 👥 User Side
- 🔍 View Available Movies
- 🎟️ Book Tickets with categorized seat pricing:
  - 💺 Economy (Seats 1–20): 20% cheaper
  - 💺 Standard (Seats 21–40): Regular price
  - 💺 Premium (Seats 41–50): 50% extra
- ❌ Cancel Booked Tickets with auto refund
- 🧾 Get booking confirmation and price breakdown

### 👨‍💼 Admin Side
- 🎞️ Add New Movies
- 🗑️ Delete Movies
- 💰 View Total Revenue
- 💾 Movies and revenue persist using local file storage

---

## 💡 Revenue Logic

- ✅ Revenue **increases** when tickets are booked
- 🔄 Revenue **decreases** when tickets are cancelled
- 🗑️ **Deleting a movie will NOT affect the revenue** (revenue already earned stays recorded)

---

## 🔐 Authentication

| Role   | Password   |
|--------|------------|
| Admin  | `admin123` |
| User   | `user123`  |

---

## 📁 File Structure

- `movies.dat` – Stores all movie data and seat availability
- `revenue.dat` – Stores total revenue
- `main.c` – Contains full source code

---

## 🧑‍💻 How to Run

1. **Compile the program:**
   ```bash
   gcc main.c -o bookingSystem

2. **Run the executable:**

./bookingSystem

⚠️ Ensure you have write permission in the current directory for .dat files to be created.

## 🚀 Future Enhancements
🎨 GUI version using GTK or Qt
📅 Movie show timings and date selection
🗃️ Integration with SQLite or MySQL database
🌐 Web-based version or Android app

## 👨‍🎓 Author
Developed by a Computer Science student as a beginner-friendly project using C language to demonstrate core programming concepts like:

- File I/O
- Structs and Arrays
- Simple Authentication
- Logical flow with menus

## 📜 License
This project is open-source for learning and personal use. Attribution appreciated!
