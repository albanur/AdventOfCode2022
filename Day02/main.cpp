#include<cstdio>
#include<cstdlib>
#include<cstring>
#include<vector>

using namespace std;

char Decrypt(char response)
{
    switch(response)
    {
        case 'A':
        case 'X':
            return 'R';
        case 'B':
        case 'Y':
            return 'P';
        case 'C':
        case 'Z':
            return 'S';
        default:
            return response;
    }
}

char DecryptAnswer(char opponent, char response)
{
    switch(opponent)
    {
        case 'R':
            return response == 'Y' ? 'R' : response == 'X' ? 'S' : 'P';
        case 'P':
            return response == 'Y' ? 'P' : response == 'X' ? 'R' : 'S';
        case 'S':
            return response == 'Y' ? 'S' : response == 'X' ? 'P' : 'R';
        default:
            return response;
    }
}

int ShapeScore(char response)
{
    switch(response)
    {
        case 'R':
            return 1;
        case 'P':
            return 2;
        case 'S':
            return 3;
        default:
            return 0;
    }
}

int RoundScore(char opponent, char response)
{
    if (opponent == response)
    {
        return 3;
    }
    else
    {
        switch(opponent)
        {
            case 'R':
                return response == 'P' ? 6 : 0;
            case 'P':
                return response == 'S' ? 6 : 0;
            case 'S':
                return response == 'R' ? 6 : 0;
            default:
                return 0;
        }
    }
}

int Score(char opponent, char response)
{
    return RoundScore(opponent, response) + ShapeScore(response);
}

int main()
{
    int score = 0;
    int current = 0;
    char buffer[10];
    char opponent;
    char response;

    while(fgets(buffer, 10, stdin) != NULL)
    {
        opponent = Decrypt(buffer[0]);
        response = DecryptAnswer(opponent, buffer[2]);

        current = Score(opponent, response);
        score += current;
    }

    printf("%i\n", score);

    return 0;
}