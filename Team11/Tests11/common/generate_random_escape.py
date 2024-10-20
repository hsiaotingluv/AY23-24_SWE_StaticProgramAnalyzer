import random

code = """
procedure Example {
  x = 2;
  z = 3;
  i = 5;
  while (i!=0) {
    x = x - 1;
    if (x==1) then {
      z = x + 1; }
    else {
      y = z + x; }
    z = z + x + i;
    call q;
    i = i - 1; }
  call p; }

procedure p {
  if (x<0) then {
    while (i>0) {
      x = z * 3 + 2 * y;
      call q;
      i = i - 1; }
    x = x + 1;
    z = x + z; }
  else {
    z = 1; }
  z = z + x + i; }

procedure q {
  if (x==1) then {
    z = x + 1; }
  else {
    x = z + x; } }
"""


def insert_random_escapes(text, percentage_chance_to_escape=0.2):
    escape_chars = ['\v', '\f', '\r\n', '\n', '\t', '\r']
    modified_code = ""
    for char in text:
        modified_code += char
        if char == ';' and random.random() < percentage_chance_to_escape:
            modified_code += random.choice(escape_chars)
    return modified_code

with open("escaped_source.txt", "w") as f:
    f.write(insert_random_escapes(code))
