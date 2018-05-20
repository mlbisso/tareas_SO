import struct

fd_out = open('bin_file.bin', 'wb')


ceros = []
nombres = ["qwerty.txt", "mauros.txt", "ninios.txt", "nanias.txt", "camelo.txt", "pacana.txt", "manzan.txt", "sandia.txt", "popopo.txt", "nonono.txt", "sisisi.txt"]

for i in range(32):
	ceros.append(0)
	ceros.append(1)

for i in range(64):
	a = ceros.pop()
	if len(nombres) > 0:
		nombre = nombres.pop()
	else:
		nombre = "undotr.txt"
	b = ord(nombre[0])
	c = ord(nombre[1])
	d = ord(nombre[2])
	e = ord(nombre[3])
	f = ord(nombre[4])
	g = ord(nombre[5])
	h = ord(nombre[6])
	i = ord(nombre[7])
	j = ord(nombre[8])
	k = ord(nombre[9])
	l = 0

	m = 0
	n = 0
	o = 1
	p = 1

	entry = struct.pack('@BBBBBBBBBBBBBBBB', a,b,c,d,e,f,g,h,i,j,k,l,m,n,o,p)
	fd_out.write(entry)
	fd_out.flush()
fd_out.close()


