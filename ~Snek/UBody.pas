unit UBody;

interface

type
  TBean = class
  public
    x, y: Integer;
    Next, Prior: TBean;
    class function Equals(x, y : Integer;Other : TBean) : Boolean;
    constructor Create(x, y: Integer);
  end;

  TBody = class
  public
    Head, Tail: TBean;
    function Check(x, y: Integer) : Boolean;
    procedure Move(x, y: Integer; Erase: Boolean);
    constructor Create(x1, y1, x2, y2: Integer);
    Destructor Destroy;
  end;

implementation

class function TBean.Equals(x, y : Integer;Other: TBean) : Boolean;
begin
  if (x = Other.x) and (y = Other.y) then Result := True else result := False;
end;

function TBody.Check(x: Integer; y: Integer): Boolean;
var
Bean : TBean;
begin
  Bean := Head;
  result := False;
  while not (Bean = Tail) do
  begin
    Bean := Bean.Next;
    if (Bean.x = x) and (Bean.y = y) then
    begin
      result := True;
      Exit;
    end;
  end;

end;

procedure TBody.Move(x: Integer; y: Integer; Erase: Boolean);
var
  Bean: TBean;
begin
  Bean := TBean.Create(x, y);
  Head.Prior := Bean;
  Bean.Next := Head;
  Head := Bean;
  if Erase then
  begin
    Bean := Tail.Prior;
    Bean.Next := nil;
    Tail.Destroy;
    Tail := Bean;
  end;
end;

destructor TBody.Destroy;
var
Bean : TBean;
begin
  Bean := Head;
  while not (Bean = Tail) do
  begin
    Bean := Bean.Next;
    Bean.Prior.Destroy;
  end;
  Bean.Destroy;
end;

constructor TBody.Create(x1: Integer; y1: Integer; x2: Integer; y2: Integer);
begin
  Head := TBean.Create(x1, y1);
  Tail := TBean.Create(x2, y2);
  Head.Next := Tail;
  Tail.Prior := Head;
end;

constructor TBean.Create(x: Integer; y: Integer);
begin
  self.x := x;
  self.y := y;
end;

end.
