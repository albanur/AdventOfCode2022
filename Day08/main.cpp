#include<cstdio>
#include<cstdlib>
#include<cstring>

using namespace std;

int main()
{
    int count = 0;
    char buffer[1024];
    char trees[128][128];
    int size;
    int size_x = 0;
    int size_y = 0;
    int i, j, k;
    bool visible_top, visible_bottom, visible_right, visible_left;
    int score_top, score_right, score_left, score_bottom;
    int score;
    int max_score = 0;

    while(fgets(buffer, 1024, stdin) != NULL)
    {
        size = strlen(buffer) - 1;
        if(size == 0)
        {
            break;
        }
        else
        {
            size_x = size;
        }

        for(int i = 0; i < size_x; i++)
        {
            trees[i][size_y] = buffer[i]-'0';
        }
        size_y++;
    }

    for(i = 0; i < size_x; i++)
    {
        for (j = 0; j < size_y; j++)
        {
            visible_top = visible_bottom = visible_right = visible_left = true;
            score_bottom = score_left = score_right = score_top = 0;

            //left
            for(k = j-1; k >=0; k--)
            {
                score_left++;
                if (trees[i][k] >= trees[i][j])
                {
                    visible_left = false;
                    break;
                }
            }

            //right
            for(k = j+1; k < size_y; k++)
            {
                score_right++;
                if (trees[i][k] >= trees[i][j])
                {
                    visible_right = false;
                    break;
                }
            }
            //top

            for(k = i-1; k >=0; k--)
            {
                score_top++;
                if (trees[k][j] >= trees[i][j])
                {
                    visible_top = false;
                    break;
                }
            }
            
            //bottom
            for(k = i+1; k < size_x; k++)
            {
                score_bottom++;
                if (trees[k][j] >= trees[i][j])
                {
                    visible_bottom = false;
                    break;
                }
            }

            if(visible_bottom || visible_left || visible_right || visible_top)
            {
                count++;
            }

            score = score_bottom * score_left * score_right * score_top;
            if(score > max_score)
            {
                max_score = score;
            }
        }
    }

    printf("%i\n", count);
    printf("%i\n", max_score);

    return 0;
}