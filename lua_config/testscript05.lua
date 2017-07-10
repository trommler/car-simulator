myCounter = 0


Main = {
    RequestId = 0x102,
    ResponseId = 0x202,
    BroadcastId = 0x300,

    Seed = {
    	[0x01] = "0x4455",
    	[0x03] = "0x6677",
    },

    Programming =
    {
        ReadDataByIdentifier =
        {
            [0x0003] = "0003",
            [0x0004] = "0004",
        }
    },

    Extended =
    {
        ReadDataByIdentifier =
        {
            [0x0005] = "0005",
            [0x0006] = "0006",
        }
    },

    Raw =
    {
        ["33 33 33"] = function(request)
            myCounter = myCounter + 1
            sendRaw("F1B")
            sendRaw(request)
            return toByteResponse(myCounter, 2)
        end,
        ["22 FA BC"] = "10 33 11",
        ["23 FA BC"] = "01 02 03 04 05 06 07",
        ["24 FA BC"] = "01 02 03 04 05 06 07 08 09"
    },

    ReadDataByIdentifier = { -- "=" on tables is necessary
        ["F1 90"] = function(request)
            myCounter = myCounter + 1
            return "ABCD"
        end,
        ["F1 24"] = "HPLA-12345-AB",
        ["F1 23"] = nil,
        ["FA BC"] = nil,
        ["1E 23"] = "231132",
    }
}
