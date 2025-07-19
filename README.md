**例:**

```markdown
```python
import os

def create_directory_if_not_exists(path):
    if not os.path.exists(path):
        os.makedirs(path)
        print(f"Directory '{path}' created.")
    else:
        print(f"Directory '{path}' already exists.")
```
# 使用例
create_directory_if_not_exists("my_new_folder")
