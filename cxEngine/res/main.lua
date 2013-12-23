

function init()

    cxEngine.setDesignSize({w=640,h=960})
    
    cxEngine.setShowBorder(true)
    
--[[
    local c = cxObject.new()
    
    c.event.func = function() io.write('test') end

    c.event.func()
    
    io.write(cxObject.typeName)
    
    local h = cxHash.new()
    h:set(1,c)
    h:set(2,c)
    h:set(3,c)
    h:set(4,c)
    h:set("55",c)
    
    cxAssert(h:length() == 5,'hash error count')
    
    h:each(function(k,v) io.write(k,'\n') end)
    
    h:each(function(k,v) io.write(k,'\n') end)
    
    local a = cxArray.new()
    a:append(c)
    a:append(c)
    a:append(c)
    cxAssert(a:length() == 3,'array error count')
    
    a:each(function(v) io.write('array + ') end)
    
    local s = cxString.create(12);
    s:print()
    
    local list = cxList.new();
    list:append(s)
]]
    
end

function main()
    local v = cxView.make({x=100,y=200,w=300,h=300})

    --cxWindowInstance():append(v)
    --local x = cxWindow.pushView('main.xml')
    --x:on('onEnter',function(root) io.write("\n\nonEnter\n\n") end)
end

function free()
    
end
