import matplotlib.pyplot as plt


dataPath = 'build/k400.txt'

chernoffDelta = {
    0.05 : 0.1827,
    0.1 : 0.1697,
    0.5 : 0.1381
}

czebyszefDelta = {
    0.05 : 0.707,
    0.1 : 0.5,
    0.5 : 0.2236
}

def getAlfaFromDelta(delta, ratio):
    inBounds = 0
    for r in ratio:
        if abs(1-r) < delta:
            inBounds += 1

    return 1 - (inBounds/len(ratio))

def findDelta(alfa, ratio):
    deltaLeft = 0
    deltaRight = 1
    currentDelta = 0.5
    currentAlfa = 1
    epsilon = 0.0001
    while abs(currentAlfa-alfa) > epsilon:
        currentDelta = (deltaLeft + deltaRight) / 2
        currentAlfa = getAlfaFromDelta(currentDelta, ratio)
        if currentAlfa > alfa:
            deltaLeft = currentDelta
        else:
            deltaRight = currentDelta
        
        
    
    return currentDelta


def createGraphForAlpha(alpha, ratios):
    
    xAxis = []
    for i in range(1, len(ratios)+1):
        xAxis.append(i)

    
    plt.scatter(xAxis,ratios,s=0.5)

    expDelta = findDelta(alpha, ratios)
    expDeltaLower = [1 - expDelta for i in range(len(ratios))]
    expDeltaUpper = [1 + expDelta for i in range(len(ratios))]

    plt.plot(xAxis, expDeltaLower, c='#FF0000', label='Calculated')
    plt.plot(xAxis, expDeltaUpper, c='#FF0000')

    chernoffLower = [1 - chernoffDelta[alpha] for i in range(len(ratios))]
    chernoffUpper = [1 + chernoffDelta[alpha] for i in range(len(ratios))]

    plt.plot(xAxis, chernoffLower, c='#00FF00', label='Chernoff')
    plt.plot(xAxis, chernoffUpper, c='#00FF00')

    czebyszeLower = [1 - czebyszefDelta[alpha] for i in range(len(ratios))]
    czebyszeUpper = [1 + czebyszefDelta[alpha] for i in range(len(ratios))]

    plt.plot(xAxis, czebyszeLower, c='#0000FF', label='Czebyszef')
    plt.plot(xAxis, czebyszeUpper, c='#0000FF')

    plt.legend(loc="upper left")

    plt.title('a = ' + str(alpha))
    plt.savefig('graph_a_' + str(alpha) + '.png')

    plt.close()
    pass

def run():
    file = open(dataPath)
    estimates = file.read().split()[1:]
    ratio = []
    for n in range(len(estimates)):
        ratio.append((n+1)/int(estimates[n]))

    createGraphForAlpha(0.05, ratio)
    createGraphForAlpha(0.1, ratio)
    createGraphForAlpha(0.5, ratio)

def check():
    file = open(dataPath)
    estimates = file.read().split()[1:]
    ratio = []
    for n in range(len(estimates)):
        ratio.append((n+1)/int(estimates[n]))
    
#check()
run()