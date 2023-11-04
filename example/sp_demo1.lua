--[[
   sample:  use sp_def() and sp_color()
]]
function sp_bas2lua( filename )
    for s in io.lines( filename ) do
        -- sp_color or sp_def
        if string.match(s, "sp_color") ~= nil then
            p1, p2, p3 = string.match(s, "^%d+%s+sp_color%((%d+),(%d+),(%d+)%)$")
            if p3 ~= nil then 
                x68k.sp_color( p1, p2, p3 )
            end
        elseif string.match(s, "c={") ~= nil then
            c=""
        elseif string.match(s, "sp_def") ~= nil then
            no = string.match(s, "^%d+%s+sp_def%((%d+),c%)$")
            x68k.sp_def(no, c, 1)
        elseif string.match(s, "^%d+%s+&H.*,$") then
            ss = string.gsub(s, "&H", "0x")
            p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14, p15, p16
             = string.match(ss, "^%d+%s+(0x%x),(0x%x),(0x%x),(0x%x),(0x%x),(0x%x),(0x%x),(0x%x),(0x%x),(0x%x),(0x%x),(0x%x),(0x%x),(0x%x),(0x%x),(0x%x),$")
            c = c .. string.char(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14, p15, p16)
        elseif string.match(s, "^%d+%s+&H.*%x$") then
            ss = string.gsub(s, "&H", "0x")
            p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14, p15, p16
             = string.match(ss, "^%d+%s+(0x%x),(0x%x),(0x%x),(0x%x),(0x%x),(0x%x),(0x%x),(0x%x),(0x%x),(0x%x),(0x%x),(0x%x),(0x%x),(0x%x),(0x%x),(0x%x)$")
            c = c .. string.char(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14, p15, p16)
        end
    end
end


function sp_demo()

    x = {}
    y = {}
    dx = {}
    dy = {}

    spmax = 50
    frame = 400

    for i=1, spmax do
        x[i] = math.random(0, 512-16)
        y[i] = math.random(0, 512-16)
        dx[i] = math.random(1, 15) - 8
        dy[i] = math.random(1, 15) - 8
    end

    bx = 0
    by = 0

    --[[ crtmode:8 512x512, 256colors, high-reso. ]]
    x68k.crtmod(8, 1)

    x68k.sp_init()
    x68k.sp_disp(1)
    x68k.bg_set(0, 0, 1)

    x68k.sp_move(1,  0, 0, 1)
    x68k.sp_move(2, 16, 0, 2)
    x68k.sp_move(3, 32, 0, 3)
    x68k.sp_move(4, 48, 0, 4)
    x68k.sp_move(5, 64, 0, 5)
    x68k.sp_move(6, 80, 0, 6)
    x68k.sp_move(7, 96, 0, 7)

    sp_bas2lua("spsetplt.bas")
    sp_bas2lua("spdefdat.bas")

    x68k.bg_fill(0, 0x107)

    st = os.time()
    for i=1, frame do
        x68k.vsync()
        x68k.bg_scroll(0, bx, by)
        for j=1 ,spmax do
            x68k.sp_move(j, x[j], y[j], j%6+1)
--          x68k.SP_REGST(j, 1, x[j]+16, y[j]+16, j%6+0x101, 3)
--          x68k.sp_set(j, x[j], y[j], j%6+0x101, 3)
        end

        bx = (bx - 1)%512
        by = (by - 1)%512
        for j=1, spmax do
            x[j] = (x[j]+dx[j])%512
            y[j] = (y[j]+dy[j])%512
        end
    end
    et = os.time()
    x68k.sp_disp(0)
    print("sp_max:", spmax, "frame:", frame,  ", time:", et-st)
end

sp_demo()

