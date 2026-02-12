# 🍝 C++ Concurrency Restaurant Simulation

A robust multi-threaded simulation of a restaurant workflow, demonstrating the **Producer-Consumer** design pattern using modern C++ features.

This project simulates a pipeline where **Waiters** take orders, **Chefs** cook them in parallel, and **Servers** deliver them to customers, all synchronized safely without race conditions or deadlocks.

## 🚀 Key Features

* **Multi-Threading:** Uses `std::thread` to run Waiter, Chef, and Server tasks simultaneously.
* **Thread Safety:** Implements `std::mutex` and `std::lock_guard` to protect shared resources (queues).
* **Synchronization:** Uses `std::condition_variable` to efficiently wake up threads only when work is available (no busy waiting).
* **Smart Pointers:** Utilizes `std::unique_ptr` and `std::move` semantics for efficient memory management and ownership transfer of objects.
* **Pipeline Architecture:** Implements a double-queue system:
    1.  `Raw Order Queue` (Waiter -> Chef)
    2.  `Ready Food Queue` (Chef -> Server)

## 🛠️ Tech Stack

* **Language:** C++ (C++11/14/17 compatible)
* **Libraries:** `<thread>`, `<mutex>`, `<condition_variable>`, `<memory>`, `<queue>`

## 🧩 System Architecture

The simulation consists of three distinct actors operating in a pipeline:

1.  **Waiter (Producer):**
    * Simulates taking orders from customers with a delay.
    * Pushes orders into the *Order Queue*.
    * Notifies the Chef.

2.  **Chef (Consumer & Producer):**
    * Waits for orders in the *Order Queue*.
    * "Cooks" the dish (simulated by `std::this_thread::sleep_for` without blocking the queue).
    * Pushes the finished dish into the *Ready Queue*.
    * Notifies the Server.

3.  **Server (Consumer):**
    * Waits for dishes in the *Ready Queue*.
    * Serves the dish to the customer.

## 📂 Code Structure

* **`Dish` (Abstract Base Class):** Defines the interface for all food items.
* **`Pizza` & `Pasta`:** Derived classes with specific preparation times and methods.
* **`RestaurantControlSystem`:** The monitor class that manages queues, mutexes, and condition variables. It acts as the bridge between threads.

## 💻 How to Run

1.  Clone the repository:
    ```bash
    git clone [https://github.com/YOUR_USERNAME/CPP-Concurrency-Restaurant.git](https://github.com/YOUR_USERNAME/CPP-Concurrency-Restaurant.git)
    ```

2.  Compile the code (ensure you link the pthread library if on Linux/Mac):
    ```bash
    g++ -o restaurant_sim main.cpp -pthread
    ```

3.  Run the executable:
    ```bash
    ./restaurant_sim
    ```

## 📸 Sample Output

```text
Restaurant is opened.
[WAITER] Margherita Pizza is ordered by a customer.
[WAITER] Plain Pasta is ordered by a customer.
  [CHEF] Margherita Pizza is baking... (Takes time)
  [CHEF] Plain Pasta is boiling... (Quick)
>>> [SERVER] Served to customer: Plain Pasta
>>> [SERVER] Served to customer: Margherita Pizza
...
!!! MANAGER SIGNAL: RESTAURANT CLOSING !!!
=== ALL STAFF LEFT. SYSTEM SHUTDOWN. ===
🤝 Contributing
Feel free to fork this project and add new features like:

Multiple chefs or waiters.

Limited oven capacity (using Semaphore).

Menu loaded from a file.

📜 License
This project is open-source and available under the MIT License.
