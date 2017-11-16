from trainer import *



if __name__ == "__main__":
    timeframe = 0.5
    parser = argparse.ArgumentParser(description='Start a simulator training server.')
    parser.add_argument('-s', '--seed', action='store_true', default=False, help='Import seed for random numbers')
    parser.add_argument('-p', '--plot', action='store_true', default=False, help='Turn on simple plot')
    args = parser.parse_args()
    PRINTOUT = vars(args)['plot']
    SEEDED = vars(args)['seed']

    connection = createSocket(4000)
    if SEEDED:
        random.seed(13)
    game = Grid()
    data = connection.recv(2048).decode("utf-8")
    if data == "start":
        connection.sendall(game.dump('init').encode("utf-8"))


    while True:
        if not game.agent.actionQueue:
            data = connection.recv(2048).decode("utf-8")
            game.agent.fetchActions(data)

        if game.win == 1:
            if PRINTOUT:
                print("win")
            connection.sendall("win".encode("utf-8"))
            break
        elif game.win == -1:
            if PRINTOUT:
                print("lose")
            connection.sendall("lose".encode("utf-8"))
            break
        elif game.win == 0:
            connection.sendall(game.dump().encode("utf-8"))

        game.updateAll()
        if PRINTOUT:
            game.simplePlot()
