import requests
import string
import random
from multiprocessing import Pool

def generate_invite_code(size=7, chars=string.ascii_uppercase + string.digits + string.ascii_lowercase):
    return ''.join(random.choice(chars) for _ in range(size))

def check_invite(code):
    response = requests.get(f'https://discordapp.com/api/v6/invites/{code}')
    if response.status_code == 200:
        return response.json()
    else:
        return None

def main():
    with Pool() as p:
        while True:
            codes = [generate_invite_code() for _ in range(p._processes)]
            results = p.map(check_invite, codes)
            for code, server in zip(codes, results):
                if server:
                    print(f"Found valid invite: {code} for server '{server['guild']['name']}'")

if __name__ == "__main__":
    main()