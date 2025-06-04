import requests

RANDOM_WORDS_URL = "https://random-word-api.vercel.app/api?length={length}&words={total}"

def get_random_words(length: int, total: int = 1) -> set[str]:
    response = requests.get(RANDOM_WORDS_URL.format(length=length, total=total))
    words = set(response.json())

    missing = total - len(words)

    while missing:
        new_response = requests.get(RANDOM_WORDS_URL.format(length=length, total=missing))
        words |= set(new_response.json())

        missing = total - len(words)

    return words
