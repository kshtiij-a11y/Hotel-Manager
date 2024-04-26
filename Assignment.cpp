#include <iostream>
#include <fstream>
#include <iomanip>
#include <string>
using namespace std;


void greetings(); 
void file_to_array(string[], double[], int[]); //Store the content of menu from the file called "FoodMenu.txt" to array

//Manager Functions
void manager_choice(string[], double[], int[], int[], int&, double&, int&, int&, int[]); //Manager Menu
void display_menu_ori(string[], double[], int[], int[]); //Displays the original menu
void display_menu_added(string[], double[], int[], int&, int&, int[]); //Displays the menu added
void update_menu(string[], double[], int[], int&, int&, int[]); //Update the original menu
void add_menu(string[], double[], int[], int&, int&, int[]); //Add new food to the existing menu
void add_food_number(int&, int&, int&); //Asks the user the number of new food to be added into the menu
void change_price(string[], double[], int[], int&, int&, int[]); //Change the price and stock of food in the updated menu
void print_new_menu(string[], double[], int&, int[]); //Prints the new menu in an output file called "NewMenu.txt"
void display_all_orders(string[], double[], int[], int[], int[]); //Display orders from all customers
void show_total_sales(string[], double[], int[], int[], int&, double&, int[]); //Display the total sales of food in a day
void return_manager_menu(char&); //Asks user (manager) if he/she wants to return to the manager menu
void show_most_popular_dish(string[] , int[]); //Displays the most popular dish of the day

//Customer Functions
void customer_choice(string[], double[], int[], int[], int[], int&, double&, double&, int[]); //Customer menu
void make_order(string[], double[], int[], int[], int[]); //Receives orders from customers
void make_payment(string[], double[], int[], int[], double&, int[]); //Prompts user (customer) to make payment on his/her purchase
void payment_method_customer(); //Asks the customer which payment method to use
void display_current_order(string[], double[], int[], int[], int[]); //Displays the order of the current customer only
void increase_sales(double[], int[], int[], double&, double&, int[]); //Increase the values for sales of current customer and all customers, accumulate the orders from all customers
void delivery_choice(); //Asks the customer for their residential address to calculate the estimated delivery time
void return_menu_above(char&); //Asks user if he/she wants to return to the main menu

//Global Variables
const int SIZE = 100; //Maximum size of all arrays
const int ORIGINAL_MENU_SIZE = 20; //The number of food in the original food menu
const double TAX = 6; //Tax rate for each purchase

int main()
{
	int high_boundary = ORIGINAL_MENU_SIZE;//To track the total items in the menu
	int low_boundary; //To store the previous value of high_boundary
	double current_sales; //Sales for current customer only
	double total_sales; //Total sales of all customers without tax
	int total_customer = 0; //Total customers that ordered food
	char who_are_you; //Profile of user
	string food_name[SIZE]; //Array to store the name of food
	double food_price[SIZE]; //Array to store the price of each food
	int food_amount_chosen[SIZE]; //Amount of each food chosen by the current customer only
	int food_number[SIZE]; //The number that corresponds to each food
	int food_amount_chosen_total[SIZE]; //Sum of the mount of each food chosen by all customers
	int food_stock[ORIGINAL_MENU_SIZE]; //Tracks the stock of each food
	
	
	//Move menu from input file into array
	file_to_array(food_name, food_price, food_stock);
	
	//Greeting notes and prompts the profile of user
	greetings();
	
	//Set the number of each food in the menu and initialise the total quantity of customer's order for each food to 0
	for(int i=0; i<SIZE; i++)
	{
		food_number[i] = i+1;
		food_amount_chosen_total[i] = 0;
	}
	
	//DeteRsine if the identity of the user is a manager or a customer
	do
	{
		cout << endl
			 << setw(64) << "PROFILE\n"
			 << setw(86) << "---------------------------------------------\n"
			 << setw(55) <<"C." << setw(15) << "Customer\n"
			 << setw(55) <<"M." << setw(14) << "Manager\n"
			 << setw(55) <<"X." << setw(19) << "Exit Program\n"
			 << setw(86) << "---------------------------------------------\n"
			 << "Choose your profile (C/M/X): ";
		cin >> who_are_you;
		system("cls");
		
		//Redirect to manager's interface if user presses 'm' or 'M' 
		if (who_are_you == 'm' || who_are_you == 'M')
		{
			//Initialise variables for the actual password and the variable for user to input the password
			string actual_password = "password";
			string user_input_password;
			
			//Check the validity of password
			cout << "Please enter password (Press X to return to previous page): ";
			cin >> user_input_password;
			system("cls");
			
			//Incorrect password
			while(actual_password!=user_input_password)
			{
				//Returns to main menu if user presses X
				if(user_input_password=="X" || user_input_password=="x")
				{
					who_are_you = 'a';
					system("cls");
					break;
				}
				cout << "Password Incorrect!\n"
					 << "Please enter the correct password (Press X to return to previous page): ";
				cin >> user_input_password;
				cout << endl;
				system("cls");
			}
			if(user_input_password=="X" || user_input_password=="x")
			continue;
			
			//Shows manager's interface
			cout << endl
				 << setw(70) << "Manager Interface\n"
				 << setw(100) << "--------------------------------------------------------------------------\n";
			manager_choice(food_name, food_price, food_amount_chosen_total, food_number, total_customer, total_sales, low_boundary, high_boundary, food_stock);
			return_menu_above(who_are_you);
		}
		//Shows customer's interface
		else if(who_are_you == 'c' || who_are_you == 'C')
		{
			cout << endl
				 << setw(70) << "Customer Interface\n"
				 << setw(100) << "--------------------------------------------------------------------------\n";
			customer_choice(food_name, food_price, food_amount_chosen, food_amount_chosen_total, food_number, total_customer, current_sales, total_sales, food_stock);
			return_menu_above(who_are_you);
			system("cls");
		}
		//Asks user to input again if the user chose a character which is not stated in the main menu
		else if(!(who_are_you == 'M' || who_are_you == 'm' || who_are_you == 'C' || who_are_you == 'c' || who_are_you == 'X' || who_are_you == 'x'))
		{
			cout << "Please enter a correct character!\n";
		}
	}while(who_are_you != 'X' && who_are_you != 'x');
	return 0;
}

// Transfer menu data from input file to array
void file_to_array(string food_name[], double food_price[], int food_stock[])
{
	ifstream inFile;
	inFile.open("FoodMenu.txt");
	
	for(int i=0; i<ORIGINAL_MENU_SIZE; i++)
	{
		getline(inFile, food_name[i], '\t');
		inFile >> food_price[i];
		inFile >> food_stock[i];
	}
	inFile.close();
}

//Greeting notes
void greetings()
{
	cout << endl;
	for(int i=0; i<4; i++)
	cout << setw(80) << "IIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIII\n";
	for(int j=0; j<15; j++)
	{
		cout << setw(68) << "IIIIIIIIII\n";
	}
	
	cout << endl
		 << setw(71) << "~WELCOME TO NOM~\n"
		 << setw(86) << "~WE SERVE THE BEST CUISINE IN TOWN~\n"
		 << setw(75) << "PRESS ENTER TO CONTINUE\n";
	cin.get();
	system("cls");
}

/*******************
Manager's Interface
********************/
// Manager's Menu
void manager_choice(string food_name[], double food_price[], int food_amount_chosen_total[], int food_number[], int& total_customer, double& total_sales, int& low_boundary, int& high_boundary, int food_stock[])
{
	char choice; //User's choice in manager's menu
	char return_m_menu; //Asks user if he/she wants to return to manager's menu
	
	do
	{
		return_m_menu = 'B';
		cout
		 << endl
		 << setw(67) << "Manager Menu\n"
		 << setw(85) << "---------------------------------------------\n"
		 << setw(55) << "A." << setw(14) << "Display Menu\n"
		 << setw(55) << "B." << setw(13) << "Update Menu\n"
		 << setw(55) << "C." << setw(13) << "Total Sales\n"
		 << setw(55) << "D." << setw(19) << "Exit Manager Menu\n"
		 << setw(85) << "---------------------------------------------\n"
		 << "Please choose (A/B/C/D): ";
		cin >> choice;
		
		//Validation of user choice
		while(choice!='A' && choice!='a' && choice!='B' && choice!='b' && choice!='C' && choice!='c'&& choice!='D' && choice!='d')
		{
			cout << "Please choose (A/B/C/D): ";
			cin >> choice;
		}
		system("cls");
		
		switch (choice)
		{
			case 'A':
			case 'a': 
						display_menu_ori(food_name, food_price, food_number, food_stock); //Display the original menu
						return_manager_menu(return_m_menu);
						break;
			case 'B': 
			case 'b': 
						update_menu(food_name, food_price, food_number, low_boundary, high_boundary, food_stock); //Update the menu
						return_manager_menu(return_m_menu);
						break;
			case 'C': 
			case 'c': 
						show_total_sales(food_name, food_price, food_amount_chosen_total, food_number, total_customer, total_sales, food_stock); //Show the total sales of all customers
						return_manager_menu(return_m_menu);
						break;
			case 'D':
			case 'd': 
						cout << "You have exited the menu.\n";
						break;
		}
	}while(!(return_m_menu=='B' || return_m_menu=='b' ));
}

//Display the content in the original menu
void display_menu_ori(string food_name[], double food_price[], int food_number[], int food_stock[])
{
	cout
	 << endl
	 << setw(20) << "MENU\n"
	 << "-----------------------------------------------------------------------\n"
	 << "Food\t\t\tNo.\t\tStock\t\tPrice(Rs)\n"
	 << "-----------------------------------------------------------------------\n";
	
	cout << fixed << setprecision(2);
	for(int i=0; i<ORIGINAL_MENU_SIZE; i++)
	{
		cout << food_name[i] << "\t\t" << food_number[i] << "\t\t" << food_stock[i] << "\t\t" << food_price[i];
	}
	cout << endl
		 << "-----------------------------------------------------------------------\n";
}

//Ask if user wants to update the content of the Menu.
void update_menu(string food_name[], double food_price[], int food_number[], int& low_boundary, int& high_boundary, int food_stock[])
{
	char update_food_list; //Asks user if he/she wants to update the food list
	
	display_menu_ori(food_name, food_price, food_number, food_stock); //Display the original menu
	cout << "Do you want to add new items to the list?(Y/N): ";
	cin >> update_food_list;
	
	//Check validity of update_food_list
	while(update_food_list!='Y' && update_food_list!='y' && update_food_list!='N' && update_food_list!='n')
	{
		cout << "Please enter Y or N only: ";
		cin >> update_food_list;
	}
	
	switch(update_food_list)
	{
		case('Y'):
		case('y'):
				add_menu(food_name, food_price, food_number, low_boundary, high_boundary, food_stock); //Add new items(food) into the menu
		case('N'):
		case('n'):
				system("cls");
				change_price(food_name, food_price, food_number, low_boundary, high_boundary, food_stock); //Change the price for all items(food) in the menu
				break;
	}
	display_menu_ori(food_name, food_price, food_number, food_stock); //Display the original menu
	display_menu_added(food_name, food_price, food_number, low_boundary, high_boundary, food_stock); //Display the new items added in the menu and their respective prices
	print_new_menu(food_name, food_price, high_boundary, food_stock);	//Outputs the updated menu in an output file called "NewMenu.txt"
}

//Get user to add new food into the menu
void add_menu(string food_name[], double food_price[], int food_number[], int& low_boundary, int& high_boundary, int food_stock[])
{
	int add_value = 0; //The number of food the user wants to add
	char update_list_choice_again; //User's decision to add more items to the menu
	
	do
	{
		add_food_number(add_value, low_boundary, high_boundary); //Gets user input on number of food to be added
		
		//Asks the user for the name and price of food to be added
		for(int i=low_boundary; i<high_boundary; i++)
		{
			//Asks user the name of food to be added
			cout << "Please add name of food " << i+1 << " with size between 8 ~ 15 characters only: ";
			cin.ignore();
			getline(cin, food_name[i]);
			//Check the validity of the number of characters in the name of food, which must be between 8 to 15 characters only
			while(!(food_name[i].size()<=15 && food_name[i].size()>=8))
			{
				cout << "Please add name of food with size between 8 ~ 15 characters only" << " : ";
				cin.ignore();
				getline(cin, food_name[i]);
			}
			//Asks user the price of food to be added
			cout << "Please add price of food " << i+1 << " : ";
			cin >> food_price[i];
			//Check the validity of the price, which must be greater than Rs 0
			while(food_price[i]<=0)
			{
				cout << "Please enter a price that is greater than RS 0!: ";
				cin >> food_price[i];
			}
			//Asks user the stock of food to be added
			cout << "Please add the stock of food " << i+1 << " : ";
			cin >> food_stock[i];
			//Check the validity of the stock, which must be greater than 0
			while(food_stock[i]<0)
			{
				cout << "Please enter a number with at least 0!: ";
				cin >> food_stock[i];
			}
			system("cls");
		}
		system("cls");
		display_menu_ori(food_name, food_price, food_number, food_stock); //Displays the original menu 
		display_menu_added(food_name, food_price, food_number, low_boundary, high_boundary, food_stock); //Displays the new items added in the menu and their respective prices
		
		//Asks user if he/she wants to continue add items to the list
		cout << "Do you still want to add more new items to the list?(Y/N): ";
		cin >> update_list_choice_again;
		while(!(update_list_choice_again=='Y' || update_list_choice_again=='y' || update_list_choice_again=='N' || update_list_choice_again=='n'))
		{
			cout << "Please enter Y or N only: ";
			cin >> update_list_choice_again;
		}
	}while(update_list_choice_again=='Y' || update_list_choice_again=='y');
	system("cls");
}

//Gets user input on number of food to be added
void add_food_number(int& add_value, int& low_boundary, int& high_boundary)
{
	char validation; //User's choice to decide if he/she wants to change the nuber for items to be added into the menu
	
	do
	{
		cout << "How many food do you want to add? (1~" << SIZE-high_boundary << "): ";
		cin.ignore();
		cin >> add_value;
		//Check if the number of food the manager wants to add is in the range of slots available
		while(!(add_value>=1 && add_value<=SIZE-high_boundary))
		{
			cout << "Please enter a value between 1~" << SIZE-high_boundary << " only: ";
			cin.ignore();
			cin >> add_value;
		}
		
		//Asks user if he/she wants to change the input value for food to be added
		cout << "The value you've entered is " << add_value << ", do you want to change?(Y/N): ";
		cin >> validation;
		while(validation!='Y' && validation!='y' && validation!='N' && validation!='n')
		{
			cout << "Please enter Y or N only: ";
			cin >> validation;
		}
	}while(validation=='Y' || validation=='y');
	
	low_boundary=high_boundary; //Store the values of high_boundary into low_boundary
	high_boundary+=add_value; //Increment the value of high_boundary to the current highest value of the menu array
	system("cls");
}

//Display the menu added 
void display_menu_added(string food_name[], double food_price[], int food_number[], int& low_boundary, int& high_boundary, int food_stock[])
{	
	cout << fixed << setprecision(2);
	for(int i=ORIGINAL_MENU_SIZE; i<high_boundary; i++)
	{
		cout << food_name[i] << "\t\t" << food_number[i] << "\t\t" << food_stock[i] << "\t\t" << food_price[i] << endl;
	}
}

//Get user to change the price and stock of the food in the menu
void change_price(string food_name[], double food_price[], int food_number[], int& low_boundary, int& high_boundary, int food_stock[])
{
	char change_price_choice; //User's decision if he/she wants to change the price of items in the menu
	int price_to_change; //The position of the food price to be changed in the menu
	char change_price_choice_again; //User's decidion if he/she wants to continue changing the price of food
	char change_stock_choice; //User's decision if he/she wants to change the price of items in the menu
	int stock_to_change; //The position of the food price to be changed in the menu
	char change_stock_choice_again; //User's decidion if he/she wants to continue changing the price of food
	
	display_menu_ori(food_name, food_price, food_number, food_stock); //Displays the original menu 
	display_menu_added(food_name, food_price, food_number, low_boundary, high_boundary, food_stock); //Displays the new items added in the menu and their respective prices
	
	//Asks user if he/she wants to change the price of items in the menu
	cout << "Do you want to change the price?(Y/N): ";
	cin.ignore();
	cin >> change_price_choice;
	while(!(change_price_choice=='Y' || change_price_choice=='y' || change_price_choice=='N' || change_price_choice=='n'))
	{
		cout << "Please enter Y or N only: ";
		cin >> change_price_choice;
	}
	system("cls");
	
	//Change the price of items in the menu
	if(change_price_choice=='Y' || change_price_choice=='y')
	{
		do
		{
			display_menu_ori(food_name, food_price, food_number, food_stock); //Displays the original menu 
			display_menu_added(food_name, food_price, food_number, low_boundary, high_boundary, food_stock); //Displays the new items added in the menu and their respective prices
			
			//Asks user for the position of the food price to be changed in the menu
			cout << "Which price do you want to change?(1~" << high_boundary << "): ";
			cin.ignore();
			cin >> price_to_change;
			while(!(price_to_change>=1 && price_to_change<=high_boundary))
			{
				cout << "Please enter a value between 1~" << high_boundary <<" only: ";
				cin >> price_to_change;
			}
			
			//Asks the user for the new price
			cout << "What is the new price?: ";
			cin >> food_price[price_to_change-1];
			//Check the validity of the price, which must be greater than Rs 0
			while(food_price[price_to_change-1]<=0)
			{
				cout << "Please enter a price that is greater than Rs 0!: ";
				cin >> food_price[price_to_change-1];
			}
			system("cls");
			display_menu_ori(food_name, food_price, food_number, food_stock); //Displays the original menu 
			display_menu_added(food_name, food_price, food_number, low_boundary, high_boundary, food_stock); //Displays the new items added in the menu and their respective prices
			
			//Asks user if he/she wants to continue changing the food price
			cout << "Do you want to continue changing the price?(Y/N): ";
			cin >> change_price_choice_again;
			while(change_price_choice_again!='Y' && change_price_choice_again!='y' && change_price_choice_again!='N' && change_price_choice_again!='n')
			{
				cout << "Please enter Y or N only: ";
				cin >> change_price_choice_again;
			}
			system("cls");
		}while(change_price_choice_again=='Y' || change_price_choice_again=='y');
	}
	
	display_menu_ori(food_name, food_price, food_number, food_stock); //Displays the original menu 
	display_menu_added(food_name, food_price, food_number, low_boundary, high_boundary, food_stock); //Displays the new items added in the menu and their respective prices
	
	//Asks user if he/she wants to change the stock of items in the menu
	cout << "Do you want to change the stock?(Y/N): ";
	cin.ignore();
	cin >> change_stock_choice;
	while(!(change_stock_choice=='Y' || change_stock_choice=='y' || change_stock_choice=='N' || change_stock_choice=='n'))
	{
		cout << "Please enter Y or N only: ";
		cin >> change_stock_choice;
	}
	system("cls");
	
	//Change the stock of items in the menu
	if(change_stock_choice=='Y' || change_stock_choice=='y')
	{
		do
		{
			display_menu_ori(food_name, food_price, food_number, food_stock); //Displays the original menu 
			display_menu_added(food_name, food_price, food_number, low_boundary, high_boundary, food_stock); //Displays the new items added in the menu and their respective prices
			
			//Asks user for the position of the food price to be changed in the menu
			cout << "Which stock do you want to change?(1~" << high_boundary << "): ";
			cin.ignore();
			cin >> stock_to_change;
			while(!(stock_to_change>=1 && stock_to_change<=high_boundary))
			{
				cout << "Please enter a value between 1~" << high_boundary <<" only: ";
				cin >> stock_to_change;
			}
			
			//Asks the user for the new price
			cout << "What is the new stock?: ";
			cin >> food_stock[stock_to_change-1];
			//Check the validity of the price, which must be greater than Rs 0
			while(food_stock[stock_to_change-1]<0)
			{
				cout << "Please enter a nonnegative value!: ";
				cin >> food_stock[stock_to_change-1];
			}
			system("cls");
			display_menu_ori(food_name, food_price, food_number, food_stock); //Displays the original menu 
			display_menu_added(food_name, food_price, food_number, low_boundary, high_boundary, food_stock); //Displays the new items added in the menu and their respective prices
			
			//Asks user if he/she wants to continue changing the food price
			cout << "Do you want to continue changing the stock?(Y/N): ";
			cin >> change_stock_choice_again;
			while(change_stock_choice_again!='Y' && change_stock_choice_again!='y' && change_stock_choice_again!='N' && change_stock_choice_again!='n')
			{
				cout << "Please enter Y or N only: ";
				cin >> change_stock_choice_again;
			}
			system("cls");
		}while(change_stock_choice_again=='Y' || change_stock_choice_again=='y');
	}
}

//Prints the updated menu into output file
void print_new_menu(string food_name[], double food_price[], int& high_boundary, int food_stock[])
{
	ofstream outFile;
	outFile.open("NewMenu.txt");
	
	//Print original menu details
	outFile << fixed << setprecision(2);
	for(int i=0; i<ORIGINAL_MENU_SIZE; i++)
	{
		outFile << food_name[i] << "\t" << food_price[i] << endl << food_stock[i];
	}
	outFile << endl;
	
	//Print added menu details
	for(int i=ORIGINAL_MENU_SIZE; i<high_boundary; i++)
	{
		outFile << food_name[i] << "\t" << food_price[i] << endl << food_stock[i] << endl;
	}
	
	cout << endl << "Updated menu printed in NewMenu.txt.\n";
	outFile.close();
}

//Show total sales and most popular dish
void show_total_sales(string food_name[], double food_price[], int food_amount_chosen_total[], int food_number[], int& total_customer, double& total_sales, int food_stock[])
{
	display_all_orders(food_name, food_price, food_amount_chosen_total, food_number, food_stock); //Displays the list of all orders from the customers
	
	cout << endl
		 << setw(32) << "TOTAL SALES (Tax Deducted)\n"
		 << "---------------------------------------------\n"
		 << "Rs " << total_sales << endl
		 << "---------------------------------------------\n"
		 << "TOTAL CUSTOMERS:  " << total_customer << endl << endl;
	show_most_popular_dish(food_name, food_amount_chosen_total); //Displays the most popular dish of the day
}

//Finds and displays the most popular dish of the day
void show_most_popular_dish(string food_name[], int food_amount_chosen_total[])
{
	int maximum = food_amount_chosen_total[0]; //Initialise the first food as the highest sales
	for(int i=0; i<ORIGINAL_MENU_SIZE; i++)
	{
		if(food_amount_chosen_total[i]>maximum)	
		maximum = food_amount_chosen_total[i];
	}
	
	int j=0;
	while(maximum!=food_amount_chosen_total[j])
	{
		j++;
	}
	
	cout << setw(27) << "MOST POPULAR DISH\n" 
		 << "---------------------------------------------";
	if(j==0)
	{
		cout << endl;
	}
	if(maximum>0)
	cout << food_name[j] << endl;
	else
	cout << "No data!\n"; //Tells user when there is no customer's purchase data
}

//Asks manager if he/she wants to go back to manager's menu
void return_manager_menu(char& return_m_menu)
{
	cout << endl
		 << setw(30) << "Return to Manager Menu?\n"
		 << "---------------------------------------------\n"
		 << "A." << setw(16) << "Manager Menu\n"
		 << "B." << setw(8) << "Exit\n"
		 << "---------------------------------------------\n"
		 << "Please choose your next location (A/B): ";
	cin.ignore();
	cin >> return_m_menu;
	
	while(!(return_m_menu=='A' || return_m_menu=='a' || return_m_menu=='B' || return_m_menu=='b'))
	{
		cout << "Please choose a valid location (A/B): ";
		cin >> return_m_menu;
	}
	system("cls");
}

/*******************
Customer's Interface
********************/
//Choice for customer
void customer_choice(string food_name[], double food_price[], int food_amount_chosen[], int food_amount_chosen_total[], int food_number[], int& total_customer, double& current_sales, double& total_sales, int food_stock[])
{
	string customer_name; //Customer's name
	char customer_order_choice; //Customer's decision if he/she wants to make an order
	
	//Get customer name and welcome customer
	cout << "May I know your name?: ";
	cin.ignore();
	getline(cin, customer_name);
	
	//Asks the customer if he/she wants to make an order
	cout << "Hello " << customer_name << "!\n"
		 << "Welcome to Nom Food Restaurant!\n"
		 << "May I take your order?(Y/N): ";
	cin >> customer_order_choice;
	
	//Validity check on customer's decision if he/she wants to make an order
	while(customer_order_choice!='Y' && customer_order_choice!='y' && customer_order_choice!='N' && customer_order_choice!='n')
	{
		cout << "Please enter Y or N only: ";
		cin >> customer_order_choice;
	}
	system("cls");
	
	//Customers wants to make an order
	if(customer_order_choice=='Y' || customer_order_choice=='y')
	{
		make_order(food_name, food_price, food_amount_chosen, food_number, food_stock); //Asks customer for his/her order
		increase_sales(food_price, food_amount_chosen, food_amount_chosen_total, current_sales, total_sales, food_stock); //Increase the values for sales of current customer and all customers, accumulate the orders from all customers
		display_current_order(food_name, food_price, food_amount_chosen, food_number, food_stock); //Displays the orders made by current customer
		delivery_choice(); //Prompts user to choose his/her residential address to estimate the delivery time
		make_payment(food_name, food_price, food_amount_chosen, food_number, current_sales, food_stock); //Asks user to make payment on the purchase
		total_customer+=1; //Increment the number of total customers that made purchase
		cout << endl
			 << "---------------------------------------------\n";
	}
}

//Asks customer for his/her order
void make_order(string food_name[], double food_price[], int food_amount_chosen[], int food_number[], int food_stock[])
{
	int customer_number_choice; //Customer's choice on which food he/she wants
	int how_many; //Quantity of the food the customer wants
	char customer_continue_choice; //Customer's decision to add or edit the choice of food or quantity of food
	
	//Set all values for the quantity of food chosen by customers to 0
	for(int i=0; i<ORIGINAL_MENU_SIZE; i++)
	food_amount_chosen[i] = 0;
	
	do
	{
		display_current_order(food_name, food_price, food_amount_chosen, food_number, food_stock); //Displays the orders made by current customer
		//Asks user for the food he/she wants to order
		cout << "Please choose food 1~" << ORIGINAL_MENU_SIZE << ": ";
		cin >> customer_number_choice;
		//Check if the choice of food is within the range of food provided
		while(!(customer_number_choice>=1 && customer_number_choice<=ORIGINAL_MENU_SIZE) || food_stock[customer_number_choice-1]==0)
		{
			if(!(customer_number_choice>=1 && customer_number_choice<=ORIGINAL_MENU_SIZE))
			{
				cout << "Please choose food 1~" << ORIGINAL_MENU_SIZE << " only: ";
			}
			else if(food_stock[customer_number_choice-1]==0)
			{
				cout << "Sorry, food " << customer_number_choice << " is out of stock, please choose another item: ";
			}
			cin >> customer_number_choice;
		}
		//Asks for the quantity of that food to be ordered
		cout << "Quantity: ";
		cin >> how_many;
		while(how_many<0 || how_many>food_stock[customer_number_choice-1])
		{
			if(how_many<0)
			{
				cout << "Please enter a quantity not less than 0!: ";
			}
			else if(how_many>food_stock[customer_number_choice-1])
			{
				cout << "Sorry, we only have " << food_stock[customer_number_choice-1] << " in stock, please enter a quantity equal or less than " << food_stock[customer_number_choice-1] << " : ";
			}
			cin >> how_many;
		}
		
		food_amount_chosen[customer_number_choice-1] = how_many;
		cout << endl;
		system("cls");
		
		display_current_order(food_name, food_price, food_amount_chosen, food_number, food_stock);//Displays the orders made by current customer
		//Asks if user wants to add more food or change the quantity of food chosen
		cout << "Do you wish to change or add more?(Y/N): ";
		cin >> customer_continue_choice;
		while(!(customer_continue_choice=='Y' || customer_continue_choice=='y' || customer_continue_choice=='N' || customer_continue_choice=='n'))
		{
			cout << "Please enter Y or N only: ";
			cin >> customer_continue_choice;
		}
		system("cls");
	}while(customer_continue_choice=='Y' || customer_continue_choice=='y');
	system("cls");
}

//To show the orders of current customer only
void display_current_order(string food_name[], double food_price[], int food_amount_chosen[], int food_number[], int food_stock[])
{
	cout << endl
		 << setw(21) << "FOOD CART\n"
		 << "-----------------------------------------------------------------------\n"
		 << "Food\t\t\tNo.\tQuantity\tStock\tPrice(Rs)\n"
		 << "-----------------------------------------------------------------------";
	
	cout << fixed << setprecision(2);
	for(int i=0; i<ORIGINAL_MENU_SIZE; i++)
	{
		if(i==0)
		cout << endl;
		cout << food_name[i] << "\t\t" << food_number[i] << "\t*" << food_amount_chosen[i] << "\t\t" << food_stock[i] << "\t" << food_price[i];
	}
	cout << "\n-----------------------------------------------------------------------\n";
}

//To show the orders of all customers
void display_all_orders(string food_name[], double food_price[], int food_amount_chosen_total[], int food_number[], int food_stock[])
{
	cout << endl
		 << setw(24) << "TOTAL ORDERS\n"
		 << "-----------------------------------------------------------------------\n"
		 << "Food\t\t\tNo.\tQuantity\tStock\tPrice(Rs)\n"
		 << "-----------------------------------------------------------------------\n";
	cout << fixed << setprecision(2);
	for(int i=0; i<ORIGINAL_MENU_SIZE; i++)
	{
		cout << food_name[i] << "\t\t" << food_number[i] << "\t*" << food_amount_chosen_total[i] << "\t\t" << food_stock[i] << "\t" << food_price[i];
	}
	cout << "\n-----------------------------------------------------------------------\n";
	cout << endl;
}

//Increase the values for sales of current customer and all customers, accumulate the orders from all customers
void increase_sales(double food_price[], int food_amount_chosen[], int food_amount_chosen_total[], double& current_sales, double& total_sales, int food_stock[])
{
	current_sales = 0;
	total_sales = 0;
	
	for(int i=0; i<ORIGINAL_MENU_SIZE; i++)
	{
		current_sales += food_price[i]*food_amount_chosen[i]; //Calculates the sales of current customer
		food_amount_chosen_total[i] += food_amount_chosen[i]; //Accumulates the total sales of each food
		total_sales += food_price[i]*food_amount_chosen_total[i]; //Calculates the total sales from all customers
		food_stock[i] -= food_amount_chosen[i];
	}
}

//Asks user for his/her residential address and calculates the estimated delivery time
void delivery_choice()
{
	const double DELIVERY_TIME = 1.17; //Distance travelled by delivery service per minute
	const double DISTANCE_BL = 12.5; //Distance between the restaurant and Batu Lanchang
	const double DISTANCE_SB = 22.3; //Distance between the restaurant and Sungai Batu
	const double DISTANCE_BF = 21.6; //Distance between the restaurant and Batu Ferringhi
	const double DISTANCE_SP = 37.8; //Distance between the restaurant and Seberang Perai
	char delivery_choice; //User's residential address
	
	cout
	 << endl
	 << setw(28) << "RESIDENTIAL ADDRESS\n"
	 << "---------------------------------------------\n"
	 << "A." << setw(16) << "Pune\n"
	 << "B." << setw(14) << "Delhi\n"
	 << "C." << setw(17) << "Gurgaon\n"
	 << "D." << setw(17) << "Bengal\n"
	 << "---------------------------------------------\n"
	 << "Please choose (A/B/C/D): ";
	cin >> delivery_choice;
	
	while(!(delivery_choice=='A' || delivery_choice=='a' || delivery_choice=='B' || delivery_choice=='b' || delivery_choice=='C' || delivery_choice=='c' || delivery_choice=='D' || delivery_choice=='d'))
	{
		cout << "Please input a valid choice!(A/B/C/D): ";
		cin >> delivery_choice;
	}
	
	cout << endl
		 << setw(30) << "ESTIMATED DELIVERY TIME\n"
		 << "-----------------------------------------------------------------\n"
		 << "Address\t\t\tDistance\t\tEstimated Time\n"
		 << "-----------------------------------------------------------------\n"
		 << fixed << setprecision(2);
	
	switch(delivery_choice)
	{
		case('A'):
	 	case('a'):
	 			cout << "Pune\t\t" << DISTANCE_BL << " KM\t\t" << DISTANCE_BL*DELIVERY_TIME << " minutes\n";
	 			break;
		case('B'):
	 	case('b'):
	 			cout << "Delhi\t\t" << DISTANCE_SB << " KM\t\t" << DISTANCE_SB*DELIVERY_TIME << " minutes\n";
	 			break;
		case('C'):
	 	case('c'):
	 			cout << "Gurgaon\t\t" << DISTANCE_BF << " KM\t\t" << DISTANCE_BF*DELIVERY_TIME << " minutes\n";
	 			break;
		case('D'):
	 	case('d'):
	 			cout << "Bengal\t\t" << DISTANCE_SP << " KM\t\t" << DISTANCE_SP*DELIVERY_TIME << " minutes\n";
	 			break;
	}
	cout << "-----------------------------------------------------------------\n";
}

//Asks user to make payment on the purchase
void make_payment(string food_name[], double food_price[], int food_amount_chosen[], int food_number[], double& current_sales, int food_stock[])
{
	double customer_payment; //Customer's payment
	double remainder; //Balance left by customer
	double payment_total = current_sales + current_sales*(TAX/100); //Total payment by customer including tax
	
	cout << fixed << setprecision(2) << endl
		 << setw(22) << "SUBTOTAL\n"
		 << "---------------------------------------------\n"
		 << "Total\t\tRs " << current_sales << endl
		 << "Tax(" << TAX << "%)\tRs " << current_sales*(TAX/100) << endl
		 << "---------------------------------------------\n"
		 << "Subtotal\tRs " << payment_total;
	payment_method_customer(); //Asks customer for a payment method
	//Asks customer to pay the sufficient amount
	cout << fixed << setprecision(2)
		 << "Please input a sufficient amount (Rs " << payment_total << "): Rs ";
	cin.ignore();
	cin >> customer_payment;
	while(!(customer_payment>=payment_total))
	{
		cout << "Please pay the sufficient amount (Rs " << payment_total << "): Rs ";
		cin >> customer_payment;
	}
	system("cls");
	
	display_current_order(food_name, food_price, food_amount_chosen, food_number, food_stock); //Displays orders made by current customer
	cout << "You have successfully made payment (Rs " << payment_total << ")\n";
	cout << "Payment: Rs " << customer_payment << endl;
	remainder = customer_payment - payment_total; //Calculates the balance of customer
	cout << "Balance: Rs " << remainder << endl;
	cout << "Thank you for your payment!";
}

/***************************************************************************************************************************************************************************************************************************************/
//Asks customer for a payment method
void payment_method_customer()
{
	char payment_method;
	
	cout << endl << endl
		 << setw(25) << "PAYMENT METHOD\n"
		 << "---------------------------------------------\n"
		 << "A." << setw(15) << "Credit Card\n"
		 << "B." << setw(14) << "Debit Card\n"
		 << "C." << setw(10) << "GPay\n"
		 << "D." << setw(23) << "Touch 'n Go Ewallet\n"
		 << "---------------------------------------------\n"
		 << "Please choose a payment method(A/B/C/D): ";
	cin.ignore();
	cin >> payment_method;
	
	while(!(payment_method=='A' || payment_method=='a' || payment_method=='B' || payment_method=='b' || payment_method=='C' || payment_method=='c' || payment_method=='D' || payment_method=='d'))
	{
		cout << "Please choose a valid payment method (A/B/C/D): ";
		cin >> payment_method;
	}
	
	switch(payment_method)
	{
		case('A'):
		case('a'):
				cout << "Credit Card\n";
				break;
		case('B'):
		case('b'):
				cout << "Debit Card\n";
				break;
		case('C'):
		case('c'):
				cout << "GPay\n";
				break;
		case('D'):
		case('d'):
				cout << "Touch 'n Go Ewallet\n";
				break;
	}
	cout << "---------------------------------------------\n";
}

//Asks user if he/she wants to return to the main menu
void return_menu_above(char& who_are_you)
{
	char return_menu_choice; //User's decidion to return to the main menu
	
	cout << endl
		 << setw(30) << "Return to Main Menu?\n"
		 << "---------------------------------------------\n"
		 << "A." << setw(13) << "Main Menu\n"
		 << "B." << setw(16) << "Exit Program\n"
		 << "---------------------------------------------\n"
		 << "Please choose your next location (A/B): ";
	cin.ignore();
	cin >> return_menu_choice;
	
	while(!(return_menu_choice=='A' || return_menu_choice=='a' || return_menu_choice=='B' || return_menu_choice=='b'))
	{
		cout << "Please choose a valid location (A/B): ";
		cin >> return_menu_choice;
	}
	system("cls");
	switch(return_menu_choice)
	{
		case('A'):
		case('a'):
				who_are_you = 'A';
				break;
		case('B'):
		case('b'):
				who_are_you = 'X';
	}
}
