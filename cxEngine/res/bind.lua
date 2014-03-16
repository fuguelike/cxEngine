
--1 self bind table
function BindSelfTestNew(this,a)
    local self = {}
    self.this = this
    self.iosLuaEventTest = function(sender,a)
        print(cxObject.GetType(self.this))
        print(cxObject.GetType(sender))
    end
    self.getWidth = function(w)
        cxLogger('getWidth invoke from self bind')
        return cxNumber.MakeFloat(100)
    end
    return self
end

--2 this's root bind table
function BindTestNew(this,a)
    local self = {}
    self.this = this
    self.iosLuaEventTest = function(sender,a)
        print(cxObject.GetType(self.this))
        print(cxObject.GetType(sender))
    end
    self.getWidth = function(w)
        cxLogger('getWidth invoke from root bind')
    end
    return self
end

--3 this's type table
function cxTable.getWidth(w)
    cxLogger('getWidth invoke from type table')
end

--4 global tabel
function getWidth(w)
    cxLogger('getWidth invoke from global')
end


