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
    -- ��Ϸ����ƫ��
    ix, iy = 0, 0
    
    -- ��ϷPID
    pid = nil
    
    -- �ϴε���ɫ��ʱ��
    lr, lg, lb, lt = 0, 0, 0, 0
    
    -- �ϴε��1vs1��ť��ʱ��
    vs = 0
    
    -- TGP����λ��
    tx, ty = 1330, 635
end

function UseTGP(x, y)
    -- �����̴�TGP
    MoveTo(x, y)
    LDClick()
    Delay(500)

    local wnd = FindWindow("��Ѷ��Ϸ�ͻ���")
    local tix, tiy = WindowPos(wnd)
    
    -- 220, 160 ǿ����ֹ��Ϸ
    -- 190, 260 ��ʼ��Ϸ
    -- 237, 12 �رհ�ť
    
    -- ��TGP����DNF
    MoveTo(190 + tix, 260 + tiy)
    LClick()
    Delay(500)

    -- ��С��TGP������
    MoveTo(237 + tix, 12 + tiy)
    LClick()
    Delay(500)
end

-- ������Ϸ����������ͨPKC
-- ����DNF.exe��PID
function StartDNF()
    -- �������DNF�������˳��ű�
    pid = FindProcess("DNF.exe")
    if pid then
        local wnd = FindWindow("���³�����ʿ")
        ix, iy = WindowPos(wnd)
        return
    end

    -- ��TGP����DNF
    UseTGP(tx, ty)
    
    -- ��ȡ����λ��
    local wnd
    while not wnd do
        wnd = FindWindow("���³�����ʿ")
        Delay(1000)
    end
    
    Delay(5000)
    ix, iy = WindowPos(wnd)
    MoveTo(10 + ix, 10 + iy)
    
    pid = FindProcess("DNF.exe")

    -- �����ϽǼ��DNF����״̬��֪������Ƶ��ѡ�����
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
    -- ѡ��һ��Ƶ��
    MoveTo(404 + ix, 347 + iy)
    LDClick()
    Delay(5000)

    -- ѡ���ɫ
    MoveTo(366 + ix, 186 + iy)
    LDClick()
    Delay(10000)
end

function EnterPKC()
    -- ����̳ǰ�ť
    MoveTo(500 + ix, 574 + iy)
    LClick()
    Delay(1000)

    -- ���˳���(Tab)�رղ˵�
    KeyPress(9)
    Delay(1000)

    -- ��P������������ѡ��˵�
    KeyPress(80)
    Delay(1000)

    -- ������ͨ������
    MoveTo(291 + ix, 301 + iy)
    LClick()
end

-- ����һ��PK���ģ��
-- �������false����Ҫ��������DNF
function PKLoopOnce()
    local r, g, b, t
    
    -- 1vs1��ť
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
    
    
    -- ��������
    MoveTo(363 + ix + math.random(-2, 2), 280 + iy + math.random(-2, 2))
    r, g, b = UnRGB(PixelColor(363 + ix, 280 + iy))
    if r == g and g == b and b == r and r ~= 0 then
        -- ��������
        MoveTo(402 + ix, 308 + iy)
        LClick()
        --Delay(10000)
        --return true
    elseif r < 50 and g < 100 and b < 160 and r > 0 and g > 60 and b > 120 then
        -- �ٴξ���
        MoveTo(363 + ix, 280 + iy)
        LClick()
        --return true
    end
    
    -- �Ƿ���
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
    
    -- PK����
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

