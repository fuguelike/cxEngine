
function init()
    cxEngine.setDesignSize({w=640,h=960})
    cxEngine.setShowBorder(true)
end

function mainBegin(xml)
    
end

function mainEnd(xml)
    
end

function main()

    local loader = cxLoading.create()

    cxLoading.event(loader,'onStart',function(this)
              local txt = cxLabelTTF.make({font='banana.ttf',size='65',text='Loading...'})
              cxView.setColor(txt,{r=1.0,g=0.0,b=0.0})
              cxLabelTTF.setFont(txt,{size=60})
              cxView.appendView(this,txt)
         end)
         
    cxLoading.event(loader,'onLoading', function(this)
              local xmlView = cxViewRoot.make('main.xml')
              cxLoading.setObject(this,xmlView)
         end)
    
    cxLoading.event(loader,'onFinished', function(this)
            local xmlView = cxLoading.getObject(this)
            cxWindow.pushView(xmlView)
         end)
         
    cxLoading.start(loader)

end

function free()
    
end
