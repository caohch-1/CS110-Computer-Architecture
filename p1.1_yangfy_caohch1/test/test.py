import json
import os
import sys

TESTS = [
  ('p1', ['simple', 'unknown_instr']),
  ('p2', ['labels']),
  ('full', ['labels', 'simple1', 'p1_errors', 'p2_errors']),
]

results = {}

for idx, (name, subtests) in enumerate(TESTS):
  for tidx, subtest in enumerate(subtests):
    fname = f'out/{name}/{subtest}.result'
    try:
      with open(fname, 'r') as f:
        ans, mem = map(int, f.readline().split())
        results[f'{idx}-{tidx}'] = ans
        if ans == 1:
          results[f'{idx}-{tidx}-mem'] = mem
        else:
          results[f'{idx}-{tidx}-mem'] = 0
    except FileNotFoundError:
        results[f'{idx}-{tidx}'] = 0
        results[f'{idx}-{tidx}-mem'] = 0

if len(sys.argv) == 2 and sys.argv[1] == '--grade':
  print(json.dumps({'scores': results}, sort_keys=True))
else:
  total = 0
  passed = 0
  for k, v in results.items():
    total += 1
    x = k.split('-')
    idx = int(x[0])
    tidx = int(x[1])
    if len(x) == 3:
      if results[f'{idx}-{tidx}'] == 1:
        if v == 1:
          passed += 1
          print('[pass]', end=' ')
        else:
          print('[FAIL]', end=' ')
      else:
        print('[----]', end=' ')
      print(f'{TESTS[idx][0]}/{TESTS[idx][1][tidx]} - Memory Check')
    else:
      if v == 1:
        passed += 1
        print('[pass]', end=' ')
      else:
        print('[FAIL]', end=' ')
      print(f'{TESTS[idx][0]}/{TESTS[idx][1][tidx]} - Answers')
  print(f'{passed} of {total} tests passed')