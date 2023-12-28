import sys
import os
import json

if __name__=="__main__":
    pwd = sys.argv[1]
    conf_file = os.path.join(pwd, ".vscode", "arduino.json")
    if os.path.exists(conf_file):
        with open(conf_file, 'r') as f:
            curr_config = json.load(f)
    
    new_conf = curr_config
    curr_config["sketch"] = sys.argv[2].removeprefix(pwd + os.path.sep)
    with open(os.path.join(pwd, ".vscode", "arduino.json"), 'w') as f:
        json.dump(curr_config, f, indent=4)