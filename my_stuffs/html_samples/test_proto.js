class X {
    constructor(obj = 0) {
	this.a = 1,
	this.b = 2,
	this.c = 3,
	this.d = 4,
	this.e = 5,
	this.f = 6,
	this.g = 7,
	this.h = 8,
	this.i = 9,
	this.j = obj,
	this.k = 11,
	this.l = 12,
	this.m = 13,
	this.n = 14,
	this.o = 15,
	this.p = 16;
    }
}

function f64_to_uint(f64)
{
        var bytes = new Uint8Array(new Float64Array([f64]).buffer)
        if(bytes.length != 8)
        {
                if(debug) log("f64_to_uint error.")
        }
        var uint = 0
        for(var i=0; i<bytes.length; i++)
        {
                uint += (bytes[bytes.length - i - 1] * Math.pow(2, 0x38 - i * 8))
        }
        return uint
}

function uint_to_f64(uint)
{
        var high  = (uint /  Math.pow(2, 0x20))
        var low   = (uint &  0xFFFFFFFF)
        var bytes = new Uint8Array((new Uint32Array([low, high]).buffer))
        var f64_array = new Float64Array(bytes.buffer)
        return f64_array[0]
}
