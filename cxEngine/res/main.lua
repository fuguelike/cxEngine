

--global metatable
--cxgEngine
--cxgWindow

function init()
    cxgEngine:setDesignSize({w=640,h=960})
    cxgEngine:setShowBorder(true)
end

function mainBegin(xml)
    
end

function mainEnd(xml)
    
end

function main()
    local loader = cxLoading.new()
    loader:on('onStart',function(loading)
              local txt = cxLabelTTF.make({font='banana.ttf',size='65',text='Loading...'})
              txt:setColor({r=1.0,g=0.0,b=0.0})
              txt:setSize(90)
              loading:append(txt)
         end)
    loader:on('onLoading', function(loading)
              local xml = cxViewRoot.make('main.xml')
              loading:setObject(xml)
         end)
    loader:on('onFinished', function(loading)
            local view = loading:object()
            cxgWindow:pushView(view)
         end)
    loader:start()
end

function free()
    
end
