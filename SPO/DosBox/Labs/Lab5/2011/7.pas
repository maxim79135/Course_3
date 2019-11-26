procedure AAA;
var V: integer;
begin
   V:=8;
end;

procedure BBB;
var K: integer;
begin
   AAA;
   K:=1;
end;

var L,M: integer;
begin
   L:=3;
   if M<22 then L:=L-1
   else M:=1;
   BBB;
end.