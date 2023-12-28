using System.Globalization;

namespace PRG182_project_2
{
    internal class Program
    {
        static List<(string, List<Sides>, int)> Orders = new();
        static void Main(string[] args)
        {
            int menu = 0;
            string name = "";
            List<Sides> sides = new();
            int quantity = 0;
            while (true)
            {
                Type Menu = Menus[menu];
                if(menu < 8)
                    DisplayMenu(Menu);
                switch (menu)
                {
                    case 0:
                        MainMenu cmd = (MainMenu)GetInput<int>("", i => i >= 1 && i <= 9, "Command must be between 1 and 9");
                        if(cmd == MainMenu.Exit0The0Program)
                            Environment.Exit(0);
                        menu = (int)cmd;
                        break;
                    case 1 or 2 or 3 or 4 or 5 or 6 or 7:
                        Array array = Enum.GetValues(Menu);
                        int food = GetInput<int>("", i => i >= 1 && i <= array.Length, $"Command must be between 1 and {array.Length}");
                        if (food == array.Length)
                        {
                            menu = 0;
                            break;
                        }
                        Enum? test = (Enum?)array.GetValue(food - 1) ?? throw new NullReferenceException();
                        name = SetUpString(test.ToString());
                        Console.Clear();
                        Console.WriteLine("{0} selected. How many would you like to order?", name);
                        quantity = GetInput<int>("", i => i >= 1, "Input must be >= 1");
                        menu = 9;
                        break;
                    case 8:
                        if(Orders.Count == 0)
                        {
                            Console.WriteLine("Cart currently empty. Place orders before checking cart. (Press key to return)");
                            menu = 0;
                            Console.ReadKey();
                            break;
                        }
                        CartEdits();
                        break;
                    case 9:
                        sides.Clear();
                        while (true)
                        {
                            Console.WriteLine("Would you like to order {0} side?(Y/N)", sides.Count > 0 ? "another" : "a");
                            if (NotNullInput("", s => new string[] { "y", "n" }.Contains(s.ToLower()), "Please enter Y or N").ToLower() != "y")
                                break;
                            Console.WriteLine("Please select a side:");
                            DisplayMenu(Menu);
                            int choice = GetInput<int>("", i => i >= 1 && i <= 6, "Input must be between 1 and 6");
                            sides.Add((Sides)choice);
                            Console.Clear();
                        }
                        Orders.Add((name,sides,quantity));
                        menu = 0;
                        break;
                }
                Console.Clear();
            }
        }
        static void CartEdits()
        {
            int menu = 0;
            List<float> calc = new();
            while(true)
            {
                if (menu != 2)
                    Console.Clear();
                switch(menu)
                {
                    case 0:
                        calc.Clear();
                        string[] Headers = { "", "Order", "Quantity", "Sides(s)", "Price" };
                        List<string[]> rows = new() { Headers };
                        int i = 1;
                        foreach ((string, List<Sides>, int) item in Orders)
                        {
                            string[] row = new string[5] { i++.ToString(), item.Item1, item.Item3.ToString(), "","" };
                            float Price = prices[row[1]] * item.Item3;
                            Dictionary<Sides, int> amounts = new();
                            foreach (Sides k in item.Item2)
                                if (amounts.ContainsKey(k))
                                    amounts[k]++;
                                else
                                    amounts[k] = 1;
                            foreach (KeyValuePair<Sides, int> pair in amounts)
                            {
                                row[3] += $"{pair.Value} x {pair.Key};";
                                Price += pair.Value * prices[SetUpString(pair.Key.ToString())];
                            }
                            
                            if (row[3] == "")
                                row[3] = "None";
                            row[4] = Price.ToString();
                            calc.Add(Price);
                            rows.Add(row);
                        }
                        string[] total = { "Total:", "", "", "", calc.Sum().ToString() };
                        rows.Add(total);
                        Console.WriteLine("\nCurrent cart: ");
                        DisplayTable(rows.ToArray());
                        Console.WriteLine("\nOptions: ");
                        DisplayMenu(Menus[8]);
                        Cart cmd = (Cart)GetInput<int>("", i => i >= 1 && i <= 3, "Input must be between 1 and 3");
                        menu = (int)cmd;
                        break;
                    case 1:
                        Console.WriteLine("Which order would you like to remove?(0 to cancel)");
                        int choice = GetInput<int>("", i => i >= 1 && i <= Orders.Count, $"Input must be between 1 and {Orders.Count}");
                        if(choice == 0)
                        {
                            menu = 0;
                            break;
                        }
                        Orders.RemoveAt(choice - 1);
                        Console.WriteLine("Order {0} removed.", choice);
                        if (Orders.Count == 0)
                        {
                            Console.WriteLine("Cart emptied. Returning to main menu on key press.");
                            Console.ReadKey();
                            return;
                        }
                        break;
                    case 2:
                        choice = GetInput<int>("Order to edit:", i => i >= 1 && i <= Orders.Count, $"Input must be between 1 and {Orders.Count}");
                        (string, List<Sides>, int) order = Orders.ElementAt(choice-1);
                        while(true)
                        {
                            Console.WriteLine("Commands");
                            DisplayMenu(Menus[10]);
                            Edit0Order property = (Edit0Order)GetInput<int>("", i => i >= 1 && i <= 4, "Input must be between 1 and 4");
                            if (property == Edit0Order.Back)
                                break;
                            switch(property)
                            {
                                case Edit0Order.Order:
                                    Console.WriteLine("1. Breakfast\n2. Combos\n3. Burgers\n4. Snacks\n5. Drinks\n6. Cooldrinks\n7. Juices\n8. Cancel edit");
                                    int root = GetInput<int>("", i => i >= 1 && i <= 8, "Input must be between 1 and 8");
                                    if (root == 8)
                                        break;
                                    DisplayMenu(Menus[root]);
                                    Array array = Enum.GetValues(Menus[root]);
                                    int food = GetInput<int>("", i => i >= 1 && i <= array.Length, $"Command must be between 1 and {array.Length}");
                                    if (food == array.Length)
                                        break;
                                    Enum? test = (Enum?)array.GetValue(food - 1) ?? throw new NullReferenceException();
                                    order.Item1 = SetUpString(test.ToString());
                                    break;
                                case Edit0Order.Quantity:
                                    Console.WriteLine("Please enter new quantity");
                                    order.Item3 = GetInput<int>("Amount:", i => i >= 1, "Input must be >= 1");
                                    break;
                                case Edit0Order.Sides:
                                    order.Item2.Clear();
                                    while (true)
                                    {
                                        if(order.Item2.Count > 0)
                                            Console.WriteLine("Would you like to order another side?(Y/N)");
                                        if (NotNullInput("", s => new string[] { "y", "n" }.Contains(s.ToLower()), "Please enter Y or N").ToLower() != "y")
                                            break;
                                        Console.WriteLine("Please select a side:");
                                        DisplayMenu(Menus[9]);
                                        int side = GetInput<int>("", i => i >= 1 && i <= 6, "Input must be between 1 and 6");
                                        order.Item2.Add((Sides)side);
                                        Console.Clear();
                                    }
                                    break;
                            }
                        }
                        (string, List<Sides>, int)[] arr = new (string, List<Sides>, int)[Orders.Count];
                        Orders.RemoveAt(choice - 1);
                        Orders.Add(order);
                        menu = 0;
                        break;
                    case 3:
                        Console.WriteLine("Total price is: {0}", calc.Sum());
                        Console.WriteLine("Do you want to check out?(Y/N)");
                        if (NotNullInput("", s => "yn".Contains(s.ToLower()), "Please use Y or N").ToLower() == "y")
                        {
                            Orders.Clear();
                            return;
                        } else
                        {
                            menu = 0;
                        }
                        break;
                }
                

            }
        }
        #region Menus
        static readonly Type[] Menus = new Type[] { typeof(MainMenu), typeof(Breakfast), typeof(Combos), typeof(Burgers), typeof(Snacks),typeof(Drinks), typeof(Cooldrinks), typeof(Juices),typeof(Cart), typeof(Sides), typeof(Edit0Order) };
        enum MainMenu
        {
            Breakfast = 1,
            Combos,
            Burgers,
            Snacks,
            Drinks,
            Cooldrinks,
            Juices,
            Cart,
            Exit0The0Program
        }
        static Dictionary<string, float> prices = new()
        {
            {"Eggs and Bacon Toast", 30 },
            {"Sausage and eggs sandwich with a side of coffee", 70 },
            {"Tea and salad sandwich",50 },
            {"Chips, Rolls and Sausage", 49 },
            {"Sausage, eggs, chips and bacon toast with a side of coffee", 99 },
            {"Burger with Chips and Cooldrink on the side", 79 },
            {"Steak and salad with pap and chutney",99 },
            {"Vetkoeks and Chips with eggs and wors",39 },
            {"Mint and vetkoeks with chips and cooldrink",25 },
            {"Beef burger", 35},
            {"Chicken Burger",40 },
            {"Hamburger",29 },
            {"Cheese Burger", 25.99f },
            {"Cooldrink",15 },
            {"Tea",10 },
            {"Black Coffee",15 },
            {"Caramel Coffee",35 },
            {"Cappuccino",30 },
            {"Juice",19.99f },
            {"Chips",13.99f },
            {"Popcorn",20 },
            {"Plain vetkoeks",20 },
            {"Slice of cake",30 },
            {"Milk Tart",25 },
            {"Koeksisters",5 },
            {"Fanta",14.99f },
            {"Coca-Cola",19.99f },
            {"Sprite",25 },
            {"Schweppes",30 },
            {"Coo-ee",29.99f },
            {"Orange Juice",9.99f },
            {"Apple Juice",14.99f },
            {"Litchi Juice",19.99f },
            {"Pineapple Juice",13.50f },
            {"Mango Juice",24.99f },
            {"Onion Rings",10 },
            {"Lettuce",5 },
            {"Bacon",10 },
            {"Water",5 }
        };
        enum Breakfast
        {
            Eggs0and0Bacon0Toast=1,
            Sausage0and0eggs0sandwich0with0a0side0of0coffee,
            Tea0and0salad0sandwich,
            Chips10Rolls0and0Sausage,
            Sausage10eggs10chips0and0bacon0toast0with0a0side0of0coffee,
            Back0To0Menu
        }
        enum Combos
        {
            Burger0with0Chips0and0Cooldrink0on0the0side=1,
            Steak0and0salad0with0pap0and0chutney,
            Vetkoeks0and0Chips0with0eggs0and0wors,
            Mint0and0vetkoeks0with0chips0and0cooldrink,
            Back0To0Menu
        }
        enum Burgers
        {
            Beef0burger=1,
            Chicken0Burger,
            Hamburger,
            Cheese0Burger,
            Back0To0Menu
        }
        enum Drinks
        { 
            Cooldrink=1, 
            Tea, 
            Black0Coffee, 
            Caramel0Coffee, 
            Cappuccino, 
            Juice,
            Back0To0Menu
        }
        enum Snacks
        {
            Chips=1,
            Popcorn,
            Plain0vetkoeks,
            Slice0of0cake,
            Milk0Tart,
            Koeksisters,
            Back0To0Menu
        }
        enum Cooldrinks
        {
            Fanta=1,
            Coca2Cola,
            Sprite,
            Schweppes,
            Coo2ee,
            Back0To0Menu
        }
        enum Juices
        {
            Orange0Juice=1,
            Apple0Juice,
            Litchi0Juice,
            Pineapple0Juice,
            Mango0Juice,
            Back0To0Menu
        }
        enum Sides
        {
            Chips=1,
            Onion0Rings,
            Lettuce,
            Bacon,
            Water
        }
        enum Cart
        {
            Remove0Order=1,
            Edit0Order,
            Checkout,
            Back0To0Menu
        }
        enum Edit0Order
        {
            Order=1,
            Quantity,
            Sides,
            Back
        }
        #endregion
        #region Input/Output helpers
        static void DisplayTable(string[][] Data)
        {
            int[] tabLengths = new int[Data[0].Length];
            for (int i = 0; i < Data.Length; i++)
                for (int j = 0; j < Data[i].Length; j++)
                {
                    int tabCount = (Data[i][j].Length + 8) / 8;
                    if (tabLengths[j] < tabCount)
                        tabLengths[j] = tabCount;
                }
            for (int i = 0; i < Data.Length; i++)
            {
                for (int j = 0; j < Data[i].Length; j++)
                {
                    Console.Write(Data[i][j]);
                    for (int k = 0; k < tabLengths[j] - Data[i][j].Length / 8; k++)
                        Console.Write("\t");
                }
                Console.WriteLine();
            }
        }
        static void DisplayMenu(Type p)
        {
            int i = 1;
            foreach (Enum item in Enum.GetValues(p))
            {
                string name = SetUpString(item.ToString());
                Console.Write("{0}. {1}", i++, name);
                if (prices.ContainsKey(name))
                    Console.Write(" = {0}", prices[name]);
                Console.WriteLine();
            }
                
        }
        static string SetUpString(string s)
        {
            string output = "";
            foreach (char c in s)
                output += c switch
                {
                    '0' => ' ',
                    '1' => ',',
                    '2' => '-',
                    _ => (object)c
                };
            return output;
        }
        static T GetInput<T>(string question, Func<T,bool>? bounds = null, string message="") where T : IParsable<T>
        {
            while(true)
            {
                string input = NotNullInput(question);
                if (!T.TryParse(input, CultureInfo.InvariantCulture, out T? output))
                    Console.WriteLine("Invalid {0} input: {1}", typeof(T).Name, input);
                else if(bounds != null)
                {
                    if (!bounds(output))
                        Console.WriteLine(message);
                    else
                        return output;
                }
                else
                    return output;
            }
        }
        static string NotNullInput(string question, Func<string, bool>? bounds = null, string message="")
        {
            while(true)
            {
                Console.Write(question);
                string? input = Console.ReadLine();
                if (input == null)
                    Console.WriteLine("Blank input detected. Please retry");
                else if(bounds != null)
                {
                    if(!bounds(input))
                        Console.WriteLine(message);
                    else return input;
                }
                else
                    return input;
            }
        }
        #endregion
    }
}