

function onInit(engine)
    local x = cxArray.new()
    x:append(cxString.create("111"))
    x:append(cxString.create("222"))
    local y = x[0]
    local z = x[1]
    cxLogger("init")
end

function onMain(engine)
    cxLogger("main");
end

function onFree(engine)
    cxLogger("free")
end

