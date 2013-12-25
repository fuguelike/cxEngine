

--global metatable

--cxgEngine
--cxgOpenGL
--cxgWindow

function init()
    cxgEngine:setDesignSize({w=640,h=960})
    cxgEngine:setShowBorder(true)
end

function mainBegin(xml)
    io.write('begin\n')
end

function mainEnd(xml)
    
end

function main()
    
    --[[
    local x = cxAssetsStream.create('items.xml');
    local b = x:allBytes()
    b:print()
     ]]
    
    local loader = cxLoading.new()
    
    --loader:setMetaMethod('test',function(self) print(self:getColor().a) end)
    --loader:test()
    
    loader:on('onStart',function(loading)
              local txt = cxLabelTTF.make({font='banana.ttf',size='65',text='Loading...'})
              txt:setColor({r=1.0,g=0.0,b=0.0})
              txt:setFont({size=60})
              loading:appendView(txt)
         end)
    loader:on('onLoading', function(loading)
              local xml = cxViewRoot.make('main.xml')
              loading:setObject(xml)
         end)
    loader:on('onFinished', function(loading)
            local view = loading:getObject()
            cxgWindow:pushView(view)
         end)
    
    loader:start()
    
    local btn = cxButton.new()
    btn:setTexture('candy.xml?green.png');
    btn:setPosition(100,200)
    cxgWindow:appendView(btn)
end

function free()
    
end
