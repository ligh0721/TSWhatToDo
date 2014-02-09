ix = 0
iy = 0

pid = nil

lr = 0
lg = 0
lb = 0
lt = 0
vs = 0

skills = {98, 99, 101, 107, 109, 111, 39, 32}

tx, ty = 0, 0

function InitGlobalVars()
    -- 游戏窗口偏移
    ix, iy = 0, 0
    
    -- 游戏PID
    pid = nil
    
    -- 上次的颜色和时间
    lr, lg, lb, lt = 0, 0, 0, 0
    
    -- 上次点击1vs1按钮的时间
    vs = 0
    
    -- TGP托盘位置
    tx, ty = 1330, 635
end

function UseTGP(x, y)
    -- 从托盘打开TGP
    MoveTo(x, y)
    LDClick()
    Delay(500)

    local wnd = FindWindow("腾讯游戏客户端")
    local tix, tiy = WindowPos(wnd)
    
    -- 220, 160 强行终止游戏
    -- 190, 260 开始游戏
    -- 237, 12 关闭按钮
    
    -- 从TGP启动DNF
    MoveTo(190 + tix, 260 + tiy)
    LClick()
    Delay(500)

    -- 最小化TGP到托盘
    MoveTo(237 + tix, 12 + tiy)
    LClick()
    Delay(500)
end

-- 启动游戏，并进入普通PKC
-- 返回DNF.exe的PID
function StartDNF()
    -- 如果存在DNF进程则退出脚本
    pid = FindProcess("DNF.exe")
    if pid then
        local wnd = FindWindow("地下城与勇士")
        ix, iy = WindowPos(wnd)
        return
    end

    -- 从TGP启动DNF
    UseTGP(tx, ty)
    
    -- 获取窗口位置
    local wnd
    while not wnd do
        wnd = FindWindow("地下城与勇士")
        Delay(1000)
    end
    
    Delay(5000)
    ix, iy = WindowPos(wnd)
    MoveTo(10 + ix, 10 + iy)
    
    pid = FindProcess("DNF.exe")

    -- 在左上角检测DNF加载状态，知道出现频道选择界面
    local loading = false;
    while loading == false or PixelColor(10 + ix, 10 + iy) == 0 do
        if loading == false and PixelColor(10 + ix, 10 + iy) == 0 then
            loading = true;
        end
        Delay(1000)
    end
    
    return pid
end

function Login()
    -- 选择一个频道
    MoveTo(404 + ix, 347 + iy)
    LDClick()
    Delay(5000)

    -- 选择角色
    MoveTo(366 + ix, 186 + iy)
    LDClick()
    Delay(10000)
end

function EnterPKC()
    -- 点击商城按钮
    MoveTo(500 + ix, 574 + iy)
    LClick()
    Delay(1000)

    -- 按退出键(Tab)关闭菜单
    KeyPress(9)
    Delay(1000)

    -- 按P键弹出决斗场选择菜单
    KeyPress(80)
    Delay(1000)

    -- 进入普通决斗场
    MoveTo(291 + ix, 301 + iy)
    LClick()
end

-- 进行一轮PK相关模拟
-- 如果返回false则需要重新启动DNF
function PKLoopOnce()
    local r, g, b, t
    
    -- 1vs1按钮
    if os.time() - vs > 120 then
        vs = os.time()
        MoveTo(710 + ix, 260 + iy)
        Delay(1000)
        r, g, b = UnRGB(PixelColor(710 + ix, 260 + iy))
        if r < 80 and b > 80 then
            MoveTo(710 + ix, 260 + iy)
            LClick()
            Delay(1000)
        end
    end
    
    
    -- 决斗结束
    MoveTo(363 + ix + math.random(-2, 2), 280 + iy + math.random(-2, 2))
    r, g, b = UnRGB(PixelColor(363 + ix, 280 + iy))
    if r == g and g == b and b == r and r ~= 0 then
        -- 更换对手
        MoveTo(402 + ix, 308 + iy)
        LClick()
        --Delay(10000)
        --return true
    elseif r < 50 and g < 100 and b < 160 and r > 0 and g > 60 and b > 120 then
        -- 再次决斗
        MoveTo(363 + ix, 280 + iy)
        LClick()
        --return true
    end
    
    -- 是否卡死
    t = os.time()
    if r ~= lr or g ~= lg or b ~= lb then
        lr, lg, lb, lt = r, g, b, t
    end
    
    Log(string.format("%u - %u = %u", t, lt, t - lt))
    if t - lt > 120 then
        if pid then
            KillProcess(pid)
        end
        return false
    end
    
    -- PK动作
    --for i, v in ipairs(skills) do
    for i = 1, #skills do
        local skill = skills[math.random(1, #skills)]
        KeyPress(skill)
    end
    
    return true
end

--[[ Test
InitGlobalVars()
Delay(1000)

pid = FindProcess("DNF.exe")
Delay(1000)

while pid do
    PKLoopOnce()
    Delay(1000)
    
    pid = FindProcess("DNF.exe")
end
os.exit(0)
]]

-- Main
math.randomseed(os.time())
while true do
    InitGlobalVars()
    Delay(1000)

    StartDNF()
    Login()
    EnterPKC()
    Delay(10000)

    while pid do
        PKLoopOnce()
        pid = FindProcess("DNF.exe")
    end
end

