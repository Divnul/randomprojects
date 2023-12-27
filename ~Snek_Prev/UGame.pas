unit UGame;

interface

uses
  Winapi.Windows, Winapi.Messages, System.SysUtils, System.Variants,
  System.Classes, Vcl.Graphics,
  Vcl.Controls, Vcl.Forms, Vcl.Dialogs, Vcl.ExtCtrls, UBody, Vcl.StdCtrls;

type
  TForm1 = class(TForm)
    PB: TPaintBox;
    tim: TTimer;
    Image1: TImage;
    lblshow: TLabel;
    procedure FormCreate(Sender: TObject);
    procedure PBPaint(Sender: TObject);
    procedure FormKeyDown(Sender: TObject; var Key: Word; Shift: TShiftState);
    procedure timTimer(Sender: TObject);
  private
    Fruit: TPoint;
    Snake: TBody;
    iDir : Integer;
    iFruits: Integer;
  public
    { Public declarations }
  end;

var
  Form1: TForm1;

const
OffArrX : array of Integer = [0,1,0,-1];
OffArrY : array of Integer = [-1,0,1,0];

implementation

{$R *.dfm}

procedure TForm1.FormCreate(Sender: TObject);
begin
  Snake := TBody.Create(2, 2, 3, 2);
  Fruit := TPoint.Create(10, 10);
  DoubleBuffered := true;
  iDir := 1;
  iFruits := 0;
end;

procedure TForm1.FormKeyDown(Sender: TObject; var Key: Word;
  Shift: TShiftState);
begin
  case Char(key) of
    'W':if not ((Snake.Head.y = Snake.Head.Next.y + 1) and (Snake.Head.x = Snake.Head.Next.x)) then
          iDir := 0;
    'S':if not ((Snake.Head.y = Snake.Head.Next.y - 1) and (Snake.Head.x = Snake.Head.Next.x)) then
          iDir := 2;
    'A':if not ((Snake.Head.x = Snake.Head.Next.x + 1) and (Snake.Head.y = Snake.Head.Next.y)) then
          iDir := 3;
    'D':if not ((Snake.Head.x = Snake.Head.Next.x - 1) and (Snake.Head.y = Snake.Head.Next.y)) then
          iDir := 1;
  end;
end;

// 38 Y 51 X 600 800 pix
procedure TForm1.PBPaint(Sender: TObject);
var
  Bean: TBean;
  i, j: Integer;
begin
  PB.Canvas.Brush.Color := $808080;
  PB.Canvas.FillRect(TRect.Create(0, 0, 816, 608));
  for i := 0 to 50 do
    for j := 0 to 37 do
    begin
      PB.Canvas.Brush.Color := $BBBBBB;
      PB.Canvas.FillRect(TRect.Create(i * 16 + 1, j * 16 + 1, i * 16 + 14,
        j * 16 + 14));
      PB.Canvas.Brush.Color := $919191;
      PB.Canvas.FillRect(TRect.Create(i * 16 + 2, j * 16 + 2, i * 16 + 12,
        j * 16 + 12));
    end;
  PB.Canvas.Brush.Color := $222277;
  PB.Canvas.FillRect(TRect.Create(Fruit.x * 16, Fruit.y * 16, Fruit.x * 16 + 16,
    Fruit.y * 16 + 16));
  PB.Canvas.Brush.Color := $5555BB;
  PB.Canvas.FillRect(TRect.Create(Fruit.x * 16 + 1, Fruit.y * 16 + 1,
    Fruit.x * 16 + 14, Fruit.y * 16 + 14));
  PB.Canvas.Brush.Color := $444487;
  PB.Canvas.FillRect(TRect.Create(Fruit.x * 16 + 2, Fruit.y * 16 + 2,
    Fruit.x * 16 + 12, Fruit.y * 16 + 12));


  Bean := Snake.Head;
  PB.Canvas.Brush.Color := $772222;
  PB.Canvas.FillRect(TRect.Create(Bean.x * 16, Bean.y * 16, Bean.x * 16 + 16,
    Bean.y * 16 + 16));
  PB.Canvas.Brush.Color := $BB5555;
  PB.Canvas.FillRect(TRect.Create(Bean.x * 16 + 1, Bean.y * 16 + 1,
    Bean.x * 16 + 14, Bean.y * 16 + 14));
  PB.Canvas.Brush.Color := $874444;
  PB.Canvas.FillRect(TRect.Create(Bean.x * 16 + 2, Bean.y * 16 + 2,
    Bean.x * 16 + 12, Bean.y * 16 + 12));

  while not(Bean = Snake.Tail) do
  begin
    Bean := Bean.Next;
    PB.Canvas.Brush.Color := $227722;

    PB.Canvas.FillRect(TRect.Create(Bean.x * 16, Bean.y * 16, Bean.x * 16 + 16,
      Bean.y * 16 + 16));
    PB.Canvas.Brush.Color := $55BB55;

    PB.Canvas.FillRect(TRect.Create(Bean.x * 16 + 1, Bean.y * 16 + 1,
      Bean.x * 16 + 14, Bean.y * 16 + 14));
    PB.Canvas.Brush.Color := $448744;

    PB.Canvas.FillRect(TRect.Create(Bean.x * 16 + 2, Bean.y * 16 + 2,
      Bean.x * 16 + 12, Bean.y * 16 + 12));
  end;

end;

procedure TForm1.timTimer(Sender: TObject);
var
  InFruit: Boolean;
  OffX, OffY: Integer;
begin
  OffX := OffArrX[iDir];
  OffY := OffArrY[iDir];
  with Snake do
  begin
    if (Head.x = Fruit.x) and (Head.y = Fruit.y) then
    begin
      Fruit := TPoint.Create(random(51), random(38));
      Inc(iFruits);
      lblshow.Caption := 'Fruits: ' + inttostr(iFruits);
      InFruit := true;
    end;
    Move((Head.x + OffX + 51) mod 51, (Head.y + OffY + 38) mod 38, not InFruit);
    InFruit := false;
    if Check(Head.x, Head.y) then
    begin
      Snake.Destroy;
      Snake := TBody.Create(2, 2, 3, 2);
      iFruits := 0;
      lblshow.Caption := 'Fruits: 0';
      iDir := 1;
    end;
  end;
  PB.RePaint;
end;

end.
