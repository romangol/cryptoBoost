import hashlib

w = hashlib.sha256()
w.update("romangol")
print w.hexdigest()
