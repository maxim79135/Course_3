<<<<<<< HEAD
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
=======
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
>>>>>>> 91850d5489a9fde5dd70a43ca26aefc90603e79e
end.