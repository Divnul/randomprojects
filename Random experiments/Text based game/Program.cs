#pragma warning disable CS8618 // Non-nullable field must contain a non-null value when exiting constructor. Consider declaring as nullable.

Console.WriteLine("You wake up on a stone floor with dim lit surroundings, you appear to be in a maze.");
int Level = 0;
int[] lvlSizes =
{
    5,10
};
while (true)
{
    int lvlSize;
    if (Level >= lvlSizes.Length)
        lvlSize = lvlSizes[lvlSizes.Length - 1];
    else
        lvlSize = lvlSizes[Level];
    GenerateLevel(lvlSize);
    GameInfo.nodes[0, 0].Arrived(2);
    string? input;
    while(true)
    {
        input = Console.ReadLine();
#pragma warning disable CS8602 // Dereference of a possibly null reference.
        string[] cmds = input.Split(' ');
#pragma warning restore CS8602 // Dereference of a possibly null reference.
        switch (cmds[0].ToLower())
        {
            #region Move cases
            case "go" or "move" or "g" or "m":
                Move(cmds[1], 1);
                break;
            case "run" or "sprint" or "r" or "spr":
                Move(cmds[1], 0);
                break;
            case "sneak" or "snk":
                Move(cmds[1], 2);
                break;
            #endregion
            case "exit":
                return;
            default:
                Console.WriteLine("Command \"{0}\" not recognized.", cmds[0]);
                break;
        }
    }
}
/*
 * Possible commands:
 * -Movement
 * -Attacking
 * -Defense
 * -Wait
 * -Pick up item
 * -Drop item
 */
#region Action Methods
bool Move(string direction, int stealth)
{
    int i = 0;
    GameInfo.Character.ActionStealth = stealth;
    for (; i < 4; i++)
        if (GameInfo.DirNames[i].ToLower() == direction.ToLower())
            break;
        else if (i == 3)
        {
            Console.WriteLine("Direction \"{0}\" doesn't exist", direction);
            return false;
        }
    return GameInfo.nodes[GameInfo.x, GameInfo.y].AttemptMove(i, stealth);
}

#endregion
void GenerateLevel(int LevelSize)
{
    GameInfo.Size = LevelSize;
    GameInfo.nodes = new Node[GameInfo.Size, GameInfo.Size];
    GameInfo.r = new Random((int)DateTime.Now.Ticks);
    for (int j = 0; j < GameInfo.Size; j++)
    {
        for (int i = 0; i < GameInfo.Size; i++)
        {
            byte b = (byte)(GameInfo.r.Next(15) + 1);
            GameInfo.nodes[i, j] = new Node(b);
            if (i > 0) GameInfo.nodes[i, j].left = GameInfo.nodes[i - 1, j].right;
            if (j > 0) GameInfo.nodes[i, j].down = GameInfo.nodes[i, j - 1].up;
        }
    }
    GameInfo.dists = new byte[GameInfo.Size, GameInfo.Size, GameInfo.Size, GameInfo.Size];
    GameInfo.dists[0, 0, 0, 0] = 1;
    int sqrCount = 0;
    int[] ls = new int[2];
    HashSet<Point> walls = new HashSet<Point>();
    while (true)
    {
        UpdateDists(ls[0], ls[1]);
        if (walls.Count == 0)
            break;
        Point w = walls.ElementAt<Point>(GameInfo.r.Next(walls.Count));
        List<int> l = new List<int>();
        for (int i = 0; i < 4; i++)
        {
            int x2 = w.x + GameInfo.Offsets[0, i];
            int y2 = w.y + GameInfo.Offsets[1, i];
            if (x2 < 0 || y2 < 0 || x2 >= GameInfo.Size || y2 >= GameInfo.Size)
                continue;
            if (GameInfo.dists[w.x + GameInfo.Offsets[0, i], w.y + GameInfo.Offsets[1, i], 0, 0] > 0)
                l.Add(i);
        }
        int dir = l.ElementAt<int>(GameInfo.r.Next(l.Count));
        int x = w.x + GameInfo.Offsets[0, dir];
        int y = w.y + GameInfo.Offsets[1, dir];
        ls[0] = w.x;
        ls[1] = w.y;
        GameInfo.nodes[w.x, w.y][dir] = true;
        GameInfo.nodes[x, y][(dir + 2) % 4] = true;
        GameInfo.dists[ls[0], ls[1], 0, 0] = (byte)(GameInfo.dists[x, y, 0, 0] + 1);
        if (sqrCount == GameInfo.Size * GameInfo.Size)
            break;
    }
    for (int i = 0; i < GameInfo.Size; i++)
        for (int j = 0; j < GameInfo.Size; j++)
        {
            if (GameInfo.dists[i, j, 0, 0] > GameInfo.MaxDist)
            {
                GameInfo.MaxDist = GameInfo.dists[i, j, 0, 0];
                GameInfo.MaxPos = new int[] { i, j };
            }
            GameInfo.dists[i, j, i, j] = 1;
            CalcDists(i, j, new int[] { i, j });
        }
    void CalcDists(int x, int y, int[] org)
    {
        for (int i = 0; i < 4; i++)
        {
            if (!GameInfo.nodes[x, y][i])
                continue;
            int x2 = x + GameInfo.Offsets[0, i];
            int y2 = y + GameInfo.Offsets[1, i];
            if (GameInfo.dists[x, y, org[0], org[1]] + 1 < GameInfo.dists[x2, y2, org[0], org[1]] || GameInfo.dists[x2, y2, org[0], org[1]] == 0)
            {
                GameInfo.dists[x2, y2, org[0], org[1]] = (byte)(GameInfo.dists[x, y, org[0], org[1]] + 1);
                CalcDists(x2, y2, org);
            }
        }
    }
    void UpdateDists(int x, int y)
    {
        sqrCount++;
        walls.Remove(new Point(x, y));
        for (int i = 0; i < 4; i++)
        {
            int x2 = x + GameInfo.Offsets[0, i];
            int y2 = y + GameInfo.Offsets[1, i];
            if (x2 < 0)
            {
                GameInfo.nodes[x, y].left = false;
                continue;
            }
            if (x2 >= GameInfo.Size)
            {
                GameInfo.nodes[x, y].right = false;
                continue;
            }
            if (y2 < 0)
            {
                GameInfo.nodes[x, y].down = false;
                continue;
            }
            if (y2 >= GameInfo.Size)
            {
                GameInfo.nodes[x, y].up = false;
                continue;
            }
            if (GameInfo.nodes[x, y][i])
            {
                if (GameInfo.dists[x, y, 0, 0] + 1 < GameInfo.dists[x2, y2, 0, 0] || GameInfo.dists[x2, y2, 0, 0] == 0)
                {
                    if (GameInfo.dists[x2, y2, 0, 0] != 0)
                        sqrCount -= 2;
                    GameInfo.dists[x2, y2, 0, 0] = (byte)(GameInfo.dists[x, y, 0, 0] + 1);
                    UpdateDists(x2, y2);
                    continue;
                }
            }
            if (GameInfo.dists[x2, y2, 0, 0] == 0)
                walls.Add(new Point(x2, y2));
        }
    }
}
static class GameInfo
{
    public static Random r;
    public static byte[,,,] dists = new byte[Size, Size, Size, Size];
    public static Node[,] nodes;
    public static int x = 0, y = 0;
    public readonly static string[] DirNames = {"East","South","West","North"};
    public static readonly int[,] Offsets =
    {
        {1,0,-1,0},
        {0,-1,0,1}
    };
    public static int Size = 0;
    public static int MaxDist = 0;
    public static int[] MaxPos = new int[2] { 0, 0 };
    public static int ProbPrecision = 100;
    public static class Character
    {
        public static float StealthStat = 1;
        public static int ActionStealth = 1;
        public static float CalcStealth(int ActionStealth)
        {
            return 2 - ActionStealth;
        }
    }
    public static bool Flip(float p)
    {
        return r.Next(ProbPrecision) < (int)(p * ProbPrecision);
    }
    public static class Events
    {
        public delegate void TimeUnit(int i);
        public static event TimeUnit timePass;
        public static void TimePass(int i) => timePass(i);
    }
    public static bool FracFlip(int top, int bottom)
    {
        return r.Next(bottom) < top;
    }
    public static List<Point> Pathfind(int[] Pos1, int[] Pos2)
    {
        List<Point> Path = new List<Point>();
        Path.Clear();
        Path.Add(new Point(Pos2[0], Pos2[1]));
        int[] pos = Pos2;
        while (!(pos[0] == Pos1[0] && pos[1] == Pos1[1]))
        {
            for(int i = 0;i< 4;i++)
            {
                if (!nodes[pos[0], pos[1]][i])
                    continue;
                int x = pos[0] + Offsets[0,i];
                int y = pos[1] + Offsets[1,i];
                if (dists[x, y,Pos1[0],Pos1[1]] < dists[pos[0], pos[1],Pos1[0],Pos1[1]])
                {
                    Path.Add(new Point(x,y));
                    pos[0] = x; pos[1] = y;
                    break;
                }
            }
        }
        Path.Reverse();
        return Path;
    }
}
struct Point
{
    public int x, y;
    public Point(int x, int y)
    {
        this.x = x; this.y = y;
    }
    public bool Equals(Point other)
    {
        return other.x == x && other.y == y;
    }
}
#region Rooms
class Node
{
    byte allowed = 0;
    public bool right
    {
        get => (allowed & 1) != 0; set => allowed = (byte)((allowed & 254) + (value ? 1 : 0));
    }
    public bool down
    {
        get => (allowed & 2) != 0; set => allowed = (byte)((allowed & 253) + (value ? 2 : 0));
    }
    public bool left
    {
        get => (allowed & 4) != 0; set => allowed = (byte)((allowed & 251) + (value ? 4 : 0));
    }
    public bool up
    {
        get => (allowed & 8) != 0; set => allowed = (byte)((allowed & 247) + (value ? 8 : 0));
    }
    public bool this[int i]
    {
        get
        {
            switch (i)
            {
                case 0:
                    return right;
                case 1:
                    return down;
                case 2:
                    return left;
                case 3:
                    return up;
                default:
                    Console.WriteLine("Index out of bounds");
                    return false;
            }
        }
        set
        {
            switch (i % 4)
            {
                case 0:
                    right = value;
                    break;
                case 1:
                    down = value;
                    break;
                case 2:
                    left = value;
                    break;
                case 3:
                    up = value;
                    break;
            }
        }
    }
    public Node(byte allowed) => this.allowed = allowed;
    public virtual void Arrived(int stealth)
    {
        List<int> dirs = new List<int>();
        for(int i = 0; i < 4; i++)
            if (this[i])
                dirs.Add(i);
        if (dirs.Count == 1)
            Console.WriteLine($"There is an exit to the {GameInfo.DirNames[dirs[0]]}");
        else
        {
            Console.Write($"There are exits to the {GameInfo.DirNames[dirs[0]]} ");
            for(int i = 1;i < dirs.Count;i++)
            {
                Console.Write(i == dirs.Count - 1 ? "and " : ", ");
                Console.Write(GameInfo.DirNames[dirs[i]] + " ");
            }
            Console.WriteLine();
        }
    }
    public virtual bool AttemptMove(int dir, int stealth)
    {
        if (this[dir])
        {
            GameInfo.x += GameInfo.Offsets[0, dir];
            GameInfo.y += GameInfo.Offsets[1, dir];
            Console.WriteLine($"Successfully moved {GameInfo.DirNames[dir]}");
            //GameInfo.Events.TimePass(2);
            GameInfo.nodes[GameInfo.x, GameInfo.y].Arrived(stealth);
            return true;
        }
        Console.WriteLine($"Couldn't move {GameInfo.DirNames[dir]}, something's in the way.");
        GameInfo.Events.TimePass(1);
        return false;
    }
    public virtual void Stayed()
    {

    }
}
class GoblinRoom : Node
{
    public int GoblinCount = 4;
    public float SpotChance;
    public bool pSpotted = false;
    struct Goblin
    {
        public int Health, MaxHealth;
        public int Damage, AtkSpeed;
        public Goblin(int health, int maxHealth, int dmg, int atkSpeed)
        {
            Health = health;
            MaxHealth = maxHealth;
            Damage = dmg;
            AtkSpeed = atkSpeed;
        }
    }
    Goblin[] Goblins;
    public GoblinRoom(byte allowed, int count, float SpotChance, int[] dmgRange, int[] atkSpeedRange, int[] HealthRange) : base(allowed)
    {
        GoblinCount = count;
        Goblins = new Goblin[GoblinCount];
        int[] BoundSizes =
        {
            dmgRange[1] - dmgRange[0],
            atkSpeedRange[1] - atkSpeedRange[0],
            HealthRange[1] - HealthRange[0]
        };
        for(int i = 0;i < GoblinCount;i++)
        {
            Goblins[i] = new Goblin();
            Goblins[i].Damage = GameInfo.r.Next(BoundSizes[0]) + dmgRange[0];
            Goblins[i].AtkSpeed = GameInfo.r.Next(BoundSizes[1]) + atkSpeedRange[1];
            Goblins[i].MaxHealth = GameInfo.r.Next(BoundSizes[2]) + HealthRange[1];
            Goblins[i].Health = Goblins[i].MaxHealth;
        }
        this.SpotChance = SpotChance;
    }
    
    public override void Arrived(int stealth)
    {
        base.Arrived(stealth);
        Console.WriteLine($"In the centre of the room lies a chest with unknown treasures. It is surrounded by {GoblinCount} goblins");
        pSpotted = GameInfo.Flip((1 - (int)Math.Pow(1 - SpotChance, GoblinCount)) * GameInfo.Character.CalcStealth(stealth));
    }
    public void PlayerSpotted()
    {
        Console.WriteLine("The goblins have spotted you and you are now engaged in battle.");
    }
}
#endregion
