from Crypto.Cipher import DES3
from Crypto import Random

def euclid(a, b):
	'''Solve x*a + y*b = ggt(a, b) and return (x, y, ggt(a, b))'''
	# Non-recursive approach hence suitable for large numbers
	x = yy = 0
	y = xx = 1
	while b:
		q = a // b
		a, b = b, a % b
		x, xx = xx - q * x, x
		y, yy = yy - q * y, y
	return xx, yy, a

def inv(a, n):
	'''Perform inversion 1/a modulo n. a and n should be COPRIME.'''
	# coprimality is not checked here in favour of performance
	i = euclid(a, n)[0]
	while i < 0:
		i += n
	return i

def test_rsa():
	p = 0xE3DB3B70833CFAC57472BE9234AECC4E0EECFD909015F199BDD109F05F89CE72B2CDAE24BD4D7923C1472DC719F9B987649C96675685D9F4EEDDA9EAE5C853EF
	q = 0xD534A891DB7899D60A917379105B9EE4589CB32046F850489539938C2EA8DF77A55BB21FA7B4FC30B2CAB67DF530918BB1267530F6A2E0C82765C5C96F27A21D
	e = 3
	n = p * q
	phi = (p-1) * (q-1) / 4
	d = inv(3, phi)
	print hex(d)

	dp = 0x1FA0B68110432192346290556C0ED5FB339B67457EEC3AED3D8EB35FEE1FA19F5272C2258E2CB795BB8E31A35E737F1C69821E77365FE09BB1C4BECFAF35F43AAD19A8D449408DD275A32625564728763E96870FAE346D606A1F356C9BB975C297568FB00FE643EAE293CB5450B8375FC128E60185E9EAE6ADFE5B7AD0614C57

	msg = 19841207
	cipher = pow( msg, e, n )
	print pow( cipher, d, n )

	print p
	print q

	phi = 0xbcf5618f955546f2490d72523f3daacd9ff09a0e160d4230b3954f206d7360ad6b466c19dac90f3e02b364e5edc99f70e68ea14f57b0d95bf366bd1c97078eb6 * 	0xde2d31b77541785812573acd705894d811e5c9dd7b013e5d0d90af437e47f6602a32cc413911a51ac038dd4e916bbbc0c9075c166cec5d108f5781337539f90a
	while phi % 3 == 0:
		phi = phi / 3
	print hex( inv(3, phi) ) 


def test_des():
	# key = b'Sixteen byte key'
	# iv = Random.new().read(DES3.block_size)
	key = "0123456789ABCDEF1111111111111111".decode("hex")
	iv = '\0' * DES3.block_size
	cipher = DES3.new(key, DES3.MODE_ECB, iv)

	data01 = "5AB745873DB3E8C6489DC3E889477FACECC21CE71C850CFEDCD46703BE812F262DEA87CD54F1C6586BB47B8208120EB8F10C28EE910D44BF216E78678ACFC1D7D79034457AB01DF0".decode("hex")

	print cipher.decrypt(data01).encode("hex")



test_des()
#test_rsa()
