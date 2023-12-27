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
    function Abs(i : Integer) : integer;
    function MineCheck(x, y : Integer): Boolean;
    procedure MineReset;
  private
    Fruit: TPoint;
    Snake: TBody;
    iDir : Integer;
    iFruits: Integer;
    ArrMines : Array[0..14] of TPoint;
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


function TForm1.Abs(i: Integer): integer;
begin
  if i > 0 then
  begin
    result := i;
    Exit;
  end;
  result := -i;

end;

procedure TForm1.FormCreate(Sender: TObject);
var
i : Integer;
begin
  Snake := TBody.Create(2, 2, 3, 2);
  Fruit := TPoint.Create(10, 10);
  DoubleBuffered := true;
  iDir := 1;
  iFruits := 0;
  MineReset;
end;

procedure TForm1.FormKeyDown(Sender: TObject; var Key: Word;
  Shift: TShiftState);
var
Dir : Integer;
begin
  Dir := iDir;
  case Key of
    VK_UP:if not ((Snake.Head.y = Snake.Head.Next.y + 1) and (Snake.Head.x = Snake.Head.Next.x)) then
          iDir := 0;
    VK_DOWN:if not ((Snake.Head.y = Snake.Head.Next.y - 1) and (Snake.Head.x = Snake.Head.Next.x)) then
          iDir := 2;
    VK_LEFT:if not ((Snake.Head.x = Snake.Head.Next.x + 1) and (Snake.Head.y = Snake.Head.Next.y)) then
          iDir := 3;
    VK_RIGHT:if not ((Snake.Head.x = Snake.Head.Next.x - 1) and (Snake.Head.y = Snake.Head.Next.y)) then
          iDir := 1;
  end;
  if Dir = iDir then
  case char(Key) of
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

function TForm1.MineCheck(x, y: Integer): Boolean;
var
i : Integer;
begin
  result := false;
  for i := 0 to 14 do
  begin
    if (ArrMines[i].x = x) AND (ArrMines[i].y = y) then
      result := true;
    if (Fruit.x = ArrMines[i].x) AND (Fruit.y = ArrMines[i].y) then
      ArrMines[i] := TPoint.Create(-1,-1);
  end;

end;

procedure TForm1.MineReset;
var
i : Integer;
begin
  for i := 0 to 14 do
  begin
    ArrMines[i] := TPoint.Create(random(48), random(36));
    if ArrMines[i].X > 23 then
      ArrMines[i].X := ArrMines[i].X + 3;
    if ArrMines[i].Y > 18 then
      ArrMines[i].Y := ArrMines[i].Y + 2;
  end;
end;

// 38 Y 51 X 600 800 pix
procedure TForm1.PBPaint(Sender: TObject);
var
  Bean: TBean;
  i, j: Integer;
  Obs : boolean;
begin
  PB.Canvas.Brush.Color := $808080;
  PB.Canvas.FillRect(TRect.Create(0, 0, 816, 608));
  PB.Canvas.Brush.Color := $333333;
  PB.Canvas.FillRect(TRect.Create(384, 0, 432, 608));
  PB.Canvas.FIllRect(TRect.Create(0,304,816,336));
  for i := 0 to 50 do
    for j := 0 to 37 do
    begin
      if (Abs(i - 25) < 2) or (j = 19) or (j = 20) then
        Obs := true
      else
        Obs := false;
      PB.Canvas.Brush.Color := $BBBBBB;
      if Obs then
        PB.Canvas.Brush.Color := $555555;
      PB.Canvas.FillRect(TRect.Create(i * 16 + 1, j * 16 + 1, i * 16 + 14,
        j * 16 + 14));
      PB.Canvas.Brush.Color := $919191;
      if Obs then
        PB.Canvas.Brush.Color := $444444;
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

  for i := 0 to 14 do
  begin
    PB.Canvas.Brush.Color := $220022;
    PB.Canvas.FillRect(Rect(ArrMines[i].x * 16, ArrMines[i].y * 16, ArrMines[i].x * 16 + 16, ArrMines[i].y * 16 + 16));
    PB.Canvas.Brush.Color := $550055;
    PB.Canvas.FillRect(Rect(ArrMines[i].x * 16+1, ArrMines[i].y * 16+1, ArrMines[i].x * 16 + 14, ArrMines[i].y * 16 + 14));
    PB.Canvas.Brush.Color := $370037;
    PB.Canvas.FillRect(Rect(ArrMines[i].x * 16+2, ArrMines[i].y * 16+2, ArrMines[i].x * 16 + 12, ArrMines[i].y * 16 + 12));
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
      Fruit := TPoint.Create(random(48), random(36));
      if Fruit.x > 23 then
        Fruit.x := Fruit.x + 3;
      if Fruit.y > 18 then
        Fruit.y := Fruit.y + 2;
      Inc(iFruits);
      lblshow.Caption := 'Fruits: ' + inttostr(iFruits);
      InFruit := true;
      if Tim.interval > 80 then
        Tim.interval := Tim.interval - 1;
    end;
    Move((Head.x + OffX + 51) mod 51, (Head.y + OffY + 38) mod 38, not InFruit);
    InFruit := false;
    if MineCheck(Head.x, Head.y) or (Check(Head.x, Head.y)) or (Abs(Head.x - 25) < 2) or (Head.y = 19) or (Head.y = 20) then
    begin
      Snake.Destroy;
      Snake := TBody.Create(2, 2, 3, 2);
      iFruits := 0;
      lblshow.Caption := 'Fruits: 0';
      iDir := 1;
      MineReset;
      Tim.Interval := 125;
    end;
  end;
  PB.RePaint;
end;

end.
