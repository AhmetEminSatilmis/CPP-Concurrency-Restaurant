#include <iostream>
#include <thread>
#include <chrono>
#include <string>
#include <memory>
#include <cstdlib>
#include <ctime>
#include <mutex>
#include <vector>
#include <queue>
#include <condition_variable>

class Dish
{
	protected:
		std::string name;
	public:
		explicit Dish(const std::string& n) : name(n) {}
		virtual ~Dish() {}

		virtual void prepare() = 0;
		std::string get_name() const
		{
			return name;
		}
};

class Pizza : public Dish
{
	public:
		explicit Pizza(const std::string& n) : Dish(n) {}
		void prepare() override
		{
			std::cout << name << " is getting prepared.[Baking]" << std::endl;
			std::this_thread::sleep_for(std::chrono::seconds(2 + std::rand() % 2));
		}
};
class Pasta : public Dish
{
	public:
		explicit Pasta(const std::string& n) : Dish(n) {}
		void prepare() override
		{
			std::cout << name << " is getting prepared.[Boiling]" << std::endl;
			std::this_thread::sleep_for(std::chrono::seconds(1 + std::rand() % 1));
		}
};

class RestaurantControlSystem
{
	private:
		std::mutex key;
		std::condition_variable chief_cv, waiter_cv;
		std::queue<std::unique_ptr<Dish>> chief_q, waiter_q;
		bool restaurant_open = true;
	public:
		void take_order(std::unique_ptr<Dish> dish)
		{
			
			std::this_thread::sleep_for(std::chrono::seconds(1));	
			std::cout << dish->get_name() << " is ordered by a customer." << std::endl;
			
			{
				std::lock_guard<std::mutex> lock(key);
				waiter_q.push(std::move(dish));
			}
			waiter_cv.notify_one();
		}
		
		void place_order()
		{
			while(true)
			{
				std::unique_lock<std::mutex> lock(key);
				waiter_cv.wait(lock, [this]()
				{
					return !restaurant_open || !waiter_q.empty();
				});
				
				if (!restaurant_open && waiter_q.empty())
				{
					std::cout << "------Waiters ceased taking order------" << std::endl;
					return;
				}
				
				if (!waiter_q.empty())
				{
					std::unique_ptr<Dish> dish = std::move(waiter_q.front());
					waiter_q.pop();
					lock.unlock();
					std::cout << dish->get_name() << " is told to chief" << std::endl;
					if (dish)
					{
						dish->prepare();
						std::cout << dish->get_name() << " is ready to serve" << std::endl;
						{
							std::unique_lock<std::mutex> lock(key);
							chief_q.push(std::move(dish));
						}
						chief_cv.notify_one();
					}
				}
			}
		}
		void serve_food()
		{
			while(true)
			{
				std::unique_lock<std::mutex> lock(key);
				chief_cv.wait(lock, [this]()
				{
					return !restaurant_open || !chief_q.empty();
				});
				if (!restaurant_open && chief_q.empty())
				{
					std::cout << "----Kitchen is closed----" << std::endl;
					return;
				}
				if (!chief_q.empty())
				{
					std::unique_ptr<Dish> dish = std::move(chief_q.front());
					chief_q.pop();
					lock.unlock();
					if (dish)
					{
						std::this_thread::sleep_for(std::chrono::seconds(1));
						std::cout << dish->get_name() << " is served to customer" << std::endl;
					}
				}
			}
		}
		void close_restaurant()
		{
			{
				std::lock_guard<std::mutex> lock(key);
				restaurant_open = false;
			}
			chief_cv.notify_all();
			waiter_cv.notify_all();
		}
};	

void waiter_order(RestaurantControlSystem& rcs)
{
	for (int i = 0; i < 3; i++)
	{
		
		
			
		rcs.take_order(std::make_unique<Pizza>("Margarita Pizza"));
		rcs.take_order(std::make_unique<Pasta>("Plain Pasta"));
		rcs.take_order(std::make_unique<Pizza>("Pepperoni Pizza"));
		rcs.take_order(std::make_unique<Pasta>("Bolognez Pasta"));
		
	}
}

int main()
{
	std::srand(static_cast<unsigned int>(std::time(nullptr)));
	std::cout << "Restaurant is opened." << std::endl;
	RestaurantControlSystem rcs;
	
	std::thread serve_thread(&RestaurantControlSystem::serve_food, &rcs);
	std::thread cook_thread(&RestaurantControlSystem::place_order, &rcs);
	std::thread order_thread(&waiter_order, std::ref(rcs));
	
	
	
	order_thread.join();
	rcs.close_restaurant();
	cook_thread.join();
	serve_thread.join();
	std::cout << "------No more food is left to serve------" << std::endl;
	
	return 0;
}
	
	




















		
		