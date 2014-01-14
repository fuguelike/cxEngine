
require('bind.lua')

function init()
    cxEngine.SetDesignSize({w=640,h=960})
    cxEngine.SetShowBorder(true)
end

function mainBegin(viewRoot)
    
end

function mainEnd(viewRoot)
    
end

--self bind table
--root bind table
--type table
--global table

function main()

    local loader = cxLoading.Create()

    cxLoading.AppendEvent(loader,'onStart',function(this)
              local txt = cxLabelTTF.Make({font='songti.ttf',size='65',text='Loading...'})
              cxView.SetColor(txt,{r=1.0,g=0.0,b=0.0})
              cxLabelTTF.SetFont(txt,{size=60})
              cxView.AppendView(this,txt)
         end)

    cxLoading.AppendEvent(loader,'onLoading', function(this)
              local xmlView = cxViewRoot.Make('main.xml')
              cxLoading.SetObject(this,xmlView)
         end)
    
    cxLoading.AppendEvent(loader,'onFinished', function(this)
            local xmlView = cxLoading.GetObject(this)
            cxWindow.PushView(xmlView)
         end)
         
    cxLoading.Start(loader)

end

function free()
    
end
