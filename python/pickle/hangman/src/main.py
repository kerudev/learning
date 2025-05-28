from game import Game

def main():
    try:
        Game().run()
    except Exception as e:
        Game().save()

if __name__ == "__main__":
    main()
