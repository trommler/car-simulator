myCounter = 0


Main = {
    RequestId = 0x102,
    ResponseId = 0x202,

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
        ["22 F1 90"] = ascii("SALGA2EV9HA298784"),
        ["33 33 33"] = function()
            myCounter = myCounter + 1
            session = 42
            --session = getCurrentSession()
            sleep(1000)
            return toByteResponse(session, 3)
        end,
        ["22 FA BC"] = "10 33 11",
        ["23 FA BC"] = "01 02 03 04 05 06 07",
        ["24 FA BC"] = "01 02 03 04 05 06 07 08 09"
    },

    ReadDataByIdentifier = { -- "=" on tables is necessary
        [0xf190] = "SALGA2EV9HA298784",
        [0xf124] = "HPLA-12345-AB",
        [0xf123] = nil,
        [0xfabc] = nil,
        [0x1e23] = "231132",
    }
}
