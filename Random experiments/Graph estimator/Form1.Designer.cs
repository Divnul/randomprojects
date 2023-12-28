using System;
using System.Diagnostics;
using System.Drawing;
using System.Security.Cryptography.X509Certificates;
using System.Threading;
using System.Windows.Forms;

namespace Graph_estimator
{
    partial class Form1
    {
        Graphics g;
        Point GraphAnchor;
        Point GraphSize;
        PointF TopLeft;
        PointF ScaleSize;
        Func<Point,double>[] functions;
        Color[] brushes;
        Thread t;
        void paint(object sender, PaintEventArgs e)
        {
            if (t.ThreadState == System.Threading.ThreadState.Stopped)
                t = new Thread(new ThreadStart(() =>
                { g.FillRectangle(new SolidBrush(BackColor), GraphAnchor.X, GraphAnchor.Y, GraphSize.X + 300, GraphSize.Y); for (int i = 0; i < functions.Length; i++) DrawFunc(functions[i], brushes[i]);
                    g.DrawString("Learn Rate", new Font("Callibri", 11), Brushes.Black, new PointF(LearnRate.Left - 130, LearnRate.Top));
                    g.DrawString("Datapoint count", new Font("Callibri", 11), Brushes.Black, new PointF(DataPointCount.Left - 130, DataPointCount.Top));
                }));
            if (!(t.ThreadState == System.Threading.ThreadState.Running))
                t.Start();
        }
        void CreateNetwork(int[] layerSizes)
        {
            network = new NeuralNetwork(layerSizes);
            gradients = new NeuralNetwork(layerSizes);
            DrawNetwork = new NeuralNetwork(layerSizes);
            gradients.ClearGradients();
            network.CopySelfTo(DrawNetwork);
        }
        void DrawFunc(Func<Point,double> func, Color c)
        {
            double[,] values = new double[GraphSize.X, GraphSize.Y];
            for(int i = 0;i < GraphSize.X; i++)
            {
                for(int j = 0; j < GraphSize.Y; j++)
                {
                    values[i, j] = func(new Point(i,j));
                    if (func == functions[1])
                    {
                        double[] outs = DrawNetwork.Calculate(convert(new Point(i, j)));
                        double value = values[i, j];
                        Color d = value < 0 ? Color.FromArgb(255 - c.R, 255 - c.G, 255 - c.B) : c;
                        value = Sigmoid(value);
                        g.FillRectangle(new SolidBrush(Color.FromArgb((byte)(value * 255), d)), GraphAnchor.X + i, GraphAnchor.Y + j, 1, 1);
                        continue;
                    }
                    if(i > 0)
                        if ((values[i, j] > 0 && values[i - 1, j] < 0) || (values[i, j] < 0 && values[i - 1, j] > 0) || values[i, j] == 0 || values[i - 1, j] == 0)
                            g.FillRectangle(new SolidBrush(c), new Rectangle(GraphAnchor.X + i, GraphAnchor.Y + j, 1, 1));
                    if (j > 0)
                        if ((values[i, j] > 0 && values[i, j - 1] < 0) || (values[i, j] < 0 && values[i, j - 1] > 0) || values[i, j] == 0 || values[i, j - 1] == 0)
                            g.FillRectangle(new SolidBrush(c), new Rectangle(GraphAnchor.X + i,GraphAnchor.Y + j, 1, 1));
                }
            }
        }
        double Sigmoid(double s)
        {
            return 1 / (1 + Math.Exp(-s));
        }
        /// <summary>
        /// Required designer variable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// Clean up any resources being used.
        /// </summary>
        /// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }
        NeuralNetwork network;
        NeuralNetwork DrawNetwork;
        NeuralNetwork gradients;
        public Form1()
        {
            InitializeComponent();
            Paint += paint;
            g = CreateGraphics();
            TopLeft = new PointF(-5, 5);
            ScaleSize = new PointF(10, 20);
            network = new NeuralNetwork(new int[] { 2, 5,5, 5, 5, 5, 5, 1 });
            DrawNetwork = new NeuralNetwork(new int[] { 2,5,5,5,  5, 5, 5, 1 });
            network.CopySelfTo(DrawNetwork);
            gradients = new NeuralNetwork(new int[] {2, 5, 5, 5, 5, 5, 5, 1 });
            gradients.ClearGradients();
            functions = new Func<Point, double>[3];
            GraphAnchor = new Point(10, 20);
            GraphSize = new Point(400, 800);
            t = new Thread(new ThreadStart(() =>
            {
                g.FillRectangle(new SolidBrush(BackColor), GraphAnchor.X, GraphAnchor.Y, GraphSize.X + 300, GraphSize.Y + 100); for (int i = 0; i < functions.Length; i++) DrawFunc(functions[i], brushes[i]);
                g.DrawString("Learn Rate", new Font("Callibri", 11), Brushes.Black, new PointF(LearnRate.Left - 130, LearnRate.Top));
                g.DrawString("Datapoint count", new Font("Callibri", 11), Brushes.Black, new PointF(DataPointCount.Left - 130, DataPointCount.Top));
            }));

            functions[0] = (Point p) =>
            {
                double[] c = convert(p);
                return Math.Sin(c[0] * Math.PI) * Math.Sin(c[1] * Math.PI);
            };
            functions[1] = (Point p) =>
            {
                double[] outs = DrawNetwork.Calculate(convert(p));
                return outs[0];
            };
            functions[2] = (Point p) =>
            {
                double[] c = convert(p);
                return c[1]*c[1] + c[0]*c[0] - 4;
            };

            brushes = new Color[3];
            brushes[1] = Color.Green;
            brushes[2] = Color.Blue;
            brushes[0] = Color.Gray;
            Train = new Button();
            Train.Text = "Train Network";
            Train.Left = GraphAnchor.X + GraphSize.X + 10;
            Train.Top = 200;
            Train.Height = 20;
            Train.Width = 150;
            Train.Parent = this;
            LearnRate = new TextBox();
            LearnRate.Top = 140;
            LearnRate.Left = GraphAnchor.X + GraphSize.X + 140;
            LearnRate.Height = 20;
            LearnRate.Width = 150;
            LearnRate.Parent = this;
            DataPointCount = new TextBox();
            DataPointCount.Top = 170;
            DataPointCount.Left = GraphAnchor.X + GraphSize.X + 140;
            DataPointCount.Height = 20;
            DataPointCount.Width = 150;
            DataPointCount.Parent = this;
            Train.Click += train;
            p = CreateGraphics();
        }
        Graphics p;
        void train(object sender, EventArgs e)
        {
            int dataCount = Convert.ToInt32(DataPointCount.Text);
            gradients.ClearGradients();
            double learnRate = Convert.ToDouble(LearnRate.Text);
            double Cost = 0;
            for(int i = 0;i < dataCount;i++)
            {
                int sign = NeuralNetwork.r.Next(2) * 2 - 1;
                Point p = new Point(NeuralNetwork.r.Next(GraphSize.X),NeuralNetwork.r.Next(GraphSize.Y));
                double[] value = network.Calculate(convert(p));
                double[] expected = functions[2](p) == 1 ? new double[] { 1, 0 } : new double[] { 0, 1 };
                Cost += Math.Pow(value[0] - expected[0], 2);
                gradients.BackPropogate(expected, network);
            }
            p.FillRectangle(new SolidBrush(BackColor), GraphAnchor.X + GraphSize.X + 10, 120, 300, 20);
            p.DrawString("Error rate:", new Font("Callibri", 11), Brushes.Black, new PointF(GraphAnchor.X + GraphSize.X + 10, 120));
            p.DrawString((Cost / dataCount).ToString(), new Font("Callibri", 11), Brushes.Black, new PointF(GraphAnchor.X + GraphSize.X + 120, 120));
            gradients.ApplyGradients(network, learnRate, dataCount);
            network.CopySelfTo(DrawNetwork);
        }
        double[] convert(Point p)
        {
            return new double[] { ((double)p.X - GraphAnchor.X) / GraphSize.X * ScaleSize.X + TopLeft.X, -((double)p.Y - GraphAnchor.Y) / GraphSize.Y * ScaleSize.Y + TopLeft.Y};
        }
        #region Windows Form Designer generated code

        /// <summary>
        /// Required method for Designer support - do not modify
        /// the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            this.components = new System.ComponentModel.Container();
            //this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(800, 450);
            this.Text = "Neural network testing";
        }
        #endregion
        Button Train;
        TextBox LearnRate, DataPointCount;
    }
    class NeuralNetwork
    {
        public delegate float FActivationFunction(float input);
        public FActivationFunction Activation;
        public void CopySelfTo(NeuralNetwork other)
        {
            for(int i = 0;i < layers[0].Nodes.Length;i++)
            {
                other.layers[0].Biases[i] = layers[0].Biases[i];
            }
            for(int i = 1; i < layers.Length;i++)
            {
                for(int j = 0;j < layers[i].Nodes.Length;j++)
                {
                    other.layers[i].Biases[j] = layers[i].Biases[j];
                    for (int k = 0; k < layers[i - 1].Nodes.Length; k++)
                    {
                        other.layers[i].Weights[k, j] = layers[i].Weights[k, j];
                    }
                }
            }
        }
        public static Random r;
        public Layer[] layers;
        public void ClearGradients()
        {
            for (int i = 0; i < layers.Length - 1; i++)
                for (int j = 0; j < layers[i].Nodes.Length; j++)
                {
                    layers[i].Biases[j] = 0;
                    for (int k = 0; k < layers[i + 1].Nodes.Length; k++)
                        layers[i + 1].Weights[j, k] = 0;
                }
        }
        public void BackPropogate(double[] expected, NeuralNetwork BaseNetwork)
        {
            Layer l = layers[layers.Length - 1];
            for(int i = 0;i < l.Nodes.Length;i++)
                l.Nodes[i] = 2 * (BaseNetwork.layers[layers.Length - 1].Nodes[i] - expected[i]);
            for(int i = layers.Length - 2;i >= 0;i--)
            {
                for(int j = 0;j < layers[i].Nodes.Length;j++)
                {
                    layers[i].Nodes[j] = 0;
                    for (int k = 0; k < layers[i + 1].Nodes.Length; k++)
                    {
                        layers[i].Nodes[j] += layers[i + 1].Nodes[k] * BaseNetwork.layers[i + 1].Weights[j, k];
                    }
                    layers[i].Nodes[j] *= BaseNetwork.layers[i].Nodes[j] * (1 - BaseNetwork.layers[i].Nodes[j]);
                    for (int k = 0; k < layers[i + 1].Nodes.Length; k++)
                    {
                        layers[i + 1].Weights[j,k] += BaseNetwork.layers[i].Nodes[j] * layers[i + 1].Nodes[k];
                    }
                    layers[i].Biases[j] += layers[i].Nodes[j];
                }
            }
        }
        public void ApplyGradients(NeuralNetwork BaseNetwork, double LearnRate, int CaseCount)
        {
            double mult = LearnRate / CaseCount;
            for(int i = 1; i < layers.Length;i++)
            {
                for(int j = 0; j < layers[i].Nodes.Length;j++)
                {
                    BaseNetwork.layers[i].Biases[j] -= layers[i].Biases[j] * mult;
                    for (int k = 0; k < layers[i - 1].Nodes.Length; k++)
                        BaseNetwork.layers[i].Weights[k, j] -= layers[i].Weights[k, j] * mult;
                }
            }
        }
        public NeuralNetwork(int[] l)
        {
            layers = new Layer[l.Length];
            r = new Random();
            layers[0] = new Layer(2, null);
            for(int i = 1;i < l.Length;i++)
                layers[i] = new Layer(l[i], layers[i-1]);
        }
        public double[] Calculate(double[] inputs)
        {
            for(int i = 0;i < inputs.Length;i++)
                layers[0].Nodes[i] = inputs[i];
            for (int i = 1; i < layers.Length; i++)
            {
                layers[i].CalculateNodes();
                if(i != layers.Length-1)
                    for(int j = 0;j < layers[i].Nodes.Length;j++)
                        layers[i].Nodes[j] = 1 / (1 - Math.Exp(-layers[i].Nodes[j]));
            }
            return layers[layers.Length - 1].Nodes;
        }
    }
    class Layer
    {
        public Layer previous;
        public double[,] Weights;
        public double[] Biases;
        public double[] Nodes;
        public Layer(int Size, Layer previous)
        {
            Nodes = new double[Size];
            Biases = new double[Size];
            Weights = new double[previous != null ? previous.Nodes.Length : 0, Size];
            this.previous = previous;
            for(int i = 0;i < Size;i++)
            {
                for(int j = 0;j < Weights.GetLength(0);j++)
                {
                    Weights[j, i] = (NeuralNetwork.r.NextDouble() * 2 - 1) / Math.Sqrt(Weights.GetLength(0));
                }
                Biases[i] = NeuralNetwork.r.NextDouble() * 2 - 1;
            }
        }
        public void CalculateNodes()
        {
            if (previous == null)
                return;
            for(int i = 0;i < Nodes.Length;i++)
            {
                Nodes[i] = Biases[i];
                for(int j = 0;j < previous.Nodes.Length;j++)
                {
                    Nodes[i] += previous.Nodes[j] * Weights[j, i];
                }
            }
        }
    }
}

