   10 int i, j, s=170, m, cnt=0
   20 m=s*s
   30 dim p(28900)
   40 print time$
   50 print "INIT TABLE"
   60 for i=2 to m
   70     p(i) = 1
   80 next
   90 /*
  100 for i=2 to s
  110     if p(i) <> 1 then goto 150
  120     print i, ",";
  130     for j=i to m/i
  140         p(i*j) = 0
  150     next
  160 next
  170 print
  180 /*
  190 print "done"
  200 print
  210 print time$
  220 for i=2 to m
  230     if p(i) = 1 then cnt=cnt+1
  240 next
  250 print cnt
  260 /*
