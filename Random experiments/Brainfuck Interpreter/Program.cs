namespace Brainfuck_Interpreter
{
    internal class Program
    {
        static void Main(string[] args)
        {
            again:
            // FIRST: Get brainfuck code from the user.
            Console.WriteLine("Enter brainfuck code to be executed:"); 
            string input = "";
            input += Console.ReadLine();

            // 1 KB playground with pointer in the middle
            byte[] cells = new byte[1024];
            int cellPointer = 512;

            // Stack structure to track loops
            Stack<int> LoopStart = new();
            Console.WriteLine("Starting Execution");
            for(int i = 0;i < input.Length;i++)
            {
                // I decided to also do some error handling in cases where things go wrong.
                switch(input[i])
                {
                    case '>':
                        cellPointer++;
                        if(cellPointer >= cells.Length) // Check if the cell pointer exited our array of values to the right.
                        {
                            Console.WriteLine("Cell pointer overflow.");
                            goto again;
                        }
                        break;
                    case '<':
                        cellPointer--;
                        if(cellPointer < 0) // Check if cell pointer exited array to the left.
                        {
                            Console.WriteLine("Cell pointer underflow.");
                            goto again;
                        }
                        break;
                    case '+':
                        cells[cellPointer]++;
                        break;
                    case '-':
                        cells[cellPointer]--;
                        break;
                    case '[':
                        if (cells[cellPointer] == 0)
                        {
                            // If the pointed value is zero, we should skip until we find this open bracket's close.
                            // This scope variable is used to enable skipping loops within loops, and also better catch missing brackets.
                            int scope = 0;
                            // This for loop might look weird. That's because I haven't explained it yet.
                            for(i++;i < input.Length;i++)
                            {
                                if (input[i] == ']')
                                {
                                    if(scope == 0)
                                    {
                                        break;
                                    }
                                    else
                                    {
                                        scope--;
                                    }
                                }
                                else if (input[i] == '[')
                                {
                                    scope++;
                                }
                            }
                            if(i == cells.Length) // This means we got to the end of the input string without finding the loop's corresponding end.
                            {
                                Console.WriteLine("Loop start at {0} lacks loop end. Press any key to exit", i);
                                Console.ReadKey();
                                goto again;
                            }
                        }
                        else
                        {
                            LoopStart.Push(i);
                        }
                        break;
                    case ']':
                        if (LoopStart.Count == 0) // This means we found a loop end after exiting all previous loops, or we haven't entered a loop yet.
                        {
                            Console.WriteLine("Loop end at {0} lacks loop start. Press any key to exit", i);
                            Console.ReadKey();
                            goto again;
                        }
                        if (cells[cellPointer] == 0)
                        {
                            LoopStart.Pop();
                        }
                        else
                        {
                            i = LoopStart.Peek();
                        }
                        break;
                    // Input/output with integers, nice if you just wanna mess with values
                    case 'i':
                        int b = 0;
                        int.TryParse(Console.ReadLine(), out b);
                        cells[cellPointer] = (byte)b;
                        break;
                    case 'o':
                        Console.WriteLine((int)cells[cellPointer]);
                        break;
                    case '.':
                        Console.Write((char)cells[cellPointer]);
                        break;
                    case ',':
                        cells[cellPointer] = (byte)Console.ReadKey().KeyChar;
                        break;
                    default: // Ignore all other characters
                        continue;
                }
            }
            if(LoopStart.Count > 0) // If the loops with missing brackets were true throughout execution, the program executes fine and ends. So I added this warning for the user, in case their code doesn't work so well in the future.
            {
                Console.WriteLine("WARNING: Unclosed loops in code.");
            }
            Console.WriteLine("\nExecution finished. Press any key to exit, or R to execute more code.");
            if(Console.ReadKey().KeyChar == 'r')
            {
                goto again;
            }
            
            // If you're interested, here's a couple fun facts about how this program will behave:
            // The ASCII character table is used to select which character to output according to the cell value.
            // Also, if you're interested in seeing this interpreter in action, copy-paste the following brainfuck code:
            // ++++++++[>++++[>++>+++>+++>+<<<<-]>+>+>->>+[<]<-]>>.>---.+++++++..+++.>>.<-.<.+++.------.--------.>>+.>++.,
            // This outputs Hello, World!, I got it from Wikipedia. Ain't that cool?
        }
    }
}