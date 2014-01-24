
require('bind.lua')

function init()
    
end

function mainBegin(viewRoot)
    cxLogger('main.xml begin')
end

function mainEnd(viewRoot)
    cxLogger('main.xml end')
end

--self bind table
--root bind table
--type table
--global table

function main()
    local loader = cxLoading.Create()
    cxLoading.On(loader,'onStart',function(this)
              local txt = cxLabelTTF.Make({font='songti.ttf',size='65',text='Loading...'})
              cxView.SetColor(txt,{r=1.0,g=0.0,b=0.0})
              cxLabelTTF.SetFont(txt,{size=60})
              cxView.AppendView(this,txt)
         end)
    cxLoading.On(loader,'onLoading', function(this)
              local xmlView = cxViewRoot.Make('main.xml')
              cxLoading.SetObject(this,xmlView)
         end)
    cxLoading.On(loader,'onFinished', function(this)
            local xmlView = cxLoading.GetObject(this)
            cxWindow.PushView(xmlView)
         end)
    cxLoading.Start(loader)
end

function free()
    
end
