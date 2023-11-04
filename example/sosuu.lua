-- calc sosuu
function sosuu(s)
  local i
  local j 
  local p = {}

  local m=s*s

  st=os.time()
  print("init table")

  for i=2, m do
    p[i]=1
  end

  for i=2, s do
    if p[i] == 1 then
      print(i)
      for j=i , m/i do
        p[i*j] = 0
      end
    end
  end
  print("done")

  print(os.time() - st)

  local cnt=0
  for j=2, m do
    if p[j] == 1 then
      cnt = cnt + 1
    end
  end
  print(cnt)
end
