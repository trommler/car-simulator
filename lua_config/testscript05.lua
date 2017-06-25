PCM = {
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

    ReadDataByIdentifier = { -- "=" on tables is necessary
        [0xf190] = "SALGA2EV9HA298784",
        [0xf124] = "HPLA-12345-AB",
        [0xf123] = nil,
        [0xfabc] = nil,
        [0x1e23] = "231132",
    }
}


