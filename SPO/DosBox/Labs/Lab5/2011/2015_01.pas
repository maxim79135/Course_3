procedure SWAP;
var A,B,T:integer;
begin
  T:=A;
  A:=B;
  B:=T;
end;
var A,B,C,D:integer;
begin
  A:=2;
  D:=1+A;
  B:=3;
  C:=B-2+(1+A);
  SWAP;
  if C>3 then D:=4
    else D:=4-D;
  A:=0;
  B:=0;
end.