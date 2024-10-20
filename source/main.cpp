#include "includes.hpp"

#define wait(time) std::this_thread::sleep_for(std::chrono::milliseconds(time))

struct cord
{
  int oX;
  int oY;
  bool life = 0;
}pl, food;

struct way_data
{
    std::vector<int> oY;
    std::vector<int> oX;
    std::vector<int> dir_list;
    int cnt = 0;
    std::vector<int> wr_dir;
}way;

std::vector<std::string> arr;

int score = 0;


void create_food()
{
    for(;;)
    {
        food.oX = (rand() % 28) + 1;
        food.oY = (rand() % 21) + 1;

        if(arr[food.oY][food.oX] == ' ')
        {
            arr[food.oY][food.oX] = '*';
            return;
        }
    }
}

bool push_step(int dir)
{
    if (dir == 0 && way.oX[way.cnt - 1] > 1)
    {
        way.oX.push_back(way.oX[way.cnt - 1] - 1);
        way.oY.push_back(way.oY[way.cnt - 1]);
        way.dir_list.push_back(0);
        return true;
    }
    else if (dir == 1 && way.oY[way.cnt - 1] > 1)
    {
        way.oX.push_back(way.oX[way.cnt - 1]);
        way.oY.push_back(way.oY[way.cnt - 1] - 1);
        way.dir_list.push_back(1);
        return true;
    }
    else if (dir == 2 && way.oX[way.cnt - 1] < 28)
    {
        way.oX.push_back(way.oX[way.cnt - 1] + 1);
        way.oY.push_back(way.oY[way.cnt - 1]);
        way.dir_list.push_back(2);
        return true;
    }
    else if(dir == 3 && way.oY[way.cnt - 1] < 21)
    {
        way.oX.push_back(way.oX[way.cnt - 1]);
        way.oY.push_back(way.oY[way.cnt - 1] + 1);
        way.dir_list.push_back(3);
        return true;
    }
    return false;
}

void create_wrong_way()
{

    for(;;)
    {
        short dir = rand() % 4;  // 0 - left ; 1 - top ; 2 - right ; 3 - down(you)

        bool rep = 0;
        for(int it = 0; it < way.wr_dir.size(); ++it)  //skip diffirent ways
        {
            if (way.wr_dir.size() == 4)
            {
                way.cnt = 1;
                int a = way.oX[0];
                int b = way.oY[0];
                int c = way.dir_list[0];
                way.oX.clear();
                way.oY.clear();
                way.dir_list.clear();
            }
            if (way.wr_dir.size() >= 3)
            {
                dir = 6 - (way.wr_dir[0] + way.wr_dir[1] + way.wr_dir[2]);
            }
            if (dir == way.wr_dir[it])
            {
                rep = 1;
                break;
            }
        }        
        if (rep)
            continue;

        /*
        Snake lock problem
                    #########
                    #    000#
                    #  000 0#
                    #########
        */

        // skip opposite dirrection 
        if (way.dir_list[way.cnt - 1] == dir + 2 || way.dir_list[way.cnt - 1] == dir - 2)
        {
            bool ok = 1;
            for(int it = 0; it < way.wr_dir.size(); ++it)
                if(dir == way.wr_dir[it]) 
                {
                    ok = 0;
                    break;
                }
            if (ok)
                way.wr_dir.push_back(dir);
            continue;
        }

        bool error = 0; // check that way is not a cross itself

        if (push_step(dir)) // if finctoin run: return true else false
        {
            for(int it = 0; it < way.cnt; ++it)
            {
                if (way.oX[way.cnt] == way.oX[it] && way.oY[way.cnt] == way.oY[it])
                {
                    error = 1;
                    break;
                }

                if ( ( (way.dir_list[way.cnt] == way.dir_list[it] - 2) ||
                       (way.dir_list[way.cnt] == way.dir_list[it] + 2)) )
                {
                    if (way.dir_list[way.cnt] % 2 == 0 && abs(way.oY[way.cnt] - way.oY[it]) <= 1)
                    {
                        error = 1;
                        break;
                    }
                    else if (way.dir_list[way.cnt] % 2 == 1 && abs(way.oX[way.cnt] - way.oX[it]) <= 1)
                    {
                        error = 1;
                        break;
                    }
                }
            }
        }       
        else
        {
            bool ok = 1;
            for(int it = 0; it < way.wr_dir.size(); ++it)
                if(dir == way.wr_dir[it]) 
                {
                    ok = 0;
                    break;
                }
            if (ok)
                way.wr_dir.push_back(dir);
            continue;
        }

        if (error)
        {
            way.oX.pop_back();
            way.oY.pop_back();
            way.dir_list.pop_back();

            bool ok = 1;
            for(int it = 0; it < way.wr_dir.size(); ++it)
                if(dir == way.wr_dir[it]) 
                {
                    ok = 0;
                    break;
                }
            if (ok)
                way.wr_dir.push_back(dir);
            continue;
        }
        else
        {
            way.wr_dir.clear();
            ++way.cnt;
            return;
        }
    }    
}


void find_wr_way()
{

    if (way.oY[way.cnt - 1] == food.oY && way.oX[way.cnt - 1] == food.oX)
        return;
    bool x_or_y = rand() % 2;
    if (x_or_y && way.oY[way.cnt - 1] != food.oY)
    {
        if (way.oY[way.cnt - 1 - 1] < food.oY)
            way.oY.push_back(way.oY[way.cnt - 1] + 1);
        else
            way.oY.push_back(way.oY[way.cnt - 1] - 1);

        way.oX.push_back(way.oX[way.cnt - 1]);
        ++way.cnt;
    }
    else
    {
        if (way.oX[way.cnt - 1 - 1] < food.oX)
            way.oX.push_back(way.oX[way.cnt - 1] + 1);
        else
            way.oX.push_back(way.oX[way.cnt - 1] - 1);

        way.oY.push_back(way.oY[way.cnt - 1]);
        ++way.cnt;
    }
}


 
void create_walls()
{
    //creator down
    bool plus_or_min = rand() % 2;
    bool dir_x_or_y = rand() % 2;
    way.oX.push_back((plus_or_min) ? pl.oX - dir_x_or_y : pl.oX + dir_x_or_y);
    way.oY.push_back((plus_or_min) ? pl.oY - !dir_x_or_y : pl.oY + !dir_x_or_y);
    ++way.cnt;

    if(plus_or_min && dir_x_or_y)
        way.dir_list.push_back(0);
    else if (!plus_or_min && dir_x_or_y)
        way.dir_list.push_back(2);
    else if(plus_or_min && dir_x_or_y)
        way.dir_list.push_back(1);
    else
        way.dir_list.push_back(3);

    

    while(way.cnt < 60)
    {
        create_wrong_way();
        for(int x = 0; x < way.cnt; ++x)
            arr[way.oY[x]][way.oX[x]] = '#';
        for(auto it : arr)
            std::cout << "   " << it << '\n';
    }



    for(;;)
    {
        if (way.oY[way.cnt - 1] == food.oY && way.oX[way.cnt - 1] == food.oX)
            break;
        find_wr_way();
    }

    for(int x = 0; x < way.cnt; ++x)
        arr[way.oY[x]][way.oX[x]] = '#';

    arr[food.oY][food.oX] = '*';
    arr[pl.oY][pl.oX] = 'O';
}

void destroy_walls()
{
    way.oX.clear();
    way.oY.clear();
    way.dir_list.clear();
    way.dir_list.clear();
    way.cnt = 0;

    for(int y = 1; y < 22; ++y)
        for(int x = 1; x < 28; ++x)
            arr[y][x] = ' ';
}

void create_player()
{
    for(;;)
    {
        pl.oX = (rand() % 28) + 1;
        pl.oY = (rand() % 21) + 1;
 
        if(arr[pl.oY][pl.oX] == ' ')
        {
            arr[pl.oY][pl.oX] = 'O';
            break;
        }
    }
}

void moving()
{
    for(;;)
    {
        std::string str;
        std::getline(std::cin,str);
        if (str.find('a') != str.npos)
        {
            arr[pl.oY][pl.oX] = ' ';
            for(;;)
            {
                if(pl.oX > 0)
                {
                    --pl.oX;
                    if (arr[pl.oY][pl.oX] == ' ' || arr[pl.oY][pl.oX] == '*')
                    {
                        if (arr[pl.oY][pl.oX] == '*')
                        {
                            food.life = false;
                            score += 25;
                        }
                        arr[pl.oY][pl.oX] = 'O';
                        break;
                    }
                    else
                    {
                        ++pl.oX;
                        arr[pl.oY][pl.oX] = 'O';
                        break;
                    }
                }
                else
                    break;
            }
        }

        if (str.find('d') != str.npos)
        {
            arr[pl.oY][pl.oX] = ' ';
            for(;;)
            {
                if(pl.oX < 29)
                {
                    ++pl.oX;
                    if (arr[pl.oY][pl.oX] == '*')
                    {
                        food.life = false;
                        score += 25;
                    }
                    if(arr[pl.oY][pl.oX] == ' ' || arr[pl.oY][pl.oX] == '*')
                    {
                        arr[pl.oY][pl.oX] = 'O';
                        break;
                    }
                    else
                    {
                        --pl.oX;
                        arr[pl.oY][pl.oX] = 'O';
                        break;
                    }
                }
                else
                    break;
            }
        }

        if (str.find('w') != str.npos)
        {
            arr[pl.oY][pl.oX] = ' ';
            for(;;)
            {
                if(pl.oY > 0)
                {
                    --pl.oY;
                    if(arr[pl.oY][pl.oX] == ' ' || arr[pl.oY][pl.oX] == '*')
                    {
                        if (arr[pl.oY][pl.oX] == '*')
                        {
                            food.life = false;
                            score += 25;
                        }
                        arr[pl.oY][pl.oX] = 'O';
                        break;
                    }
                    else
                    {
                        ++pl.oY;
                        arr[pl.oY][pl.oX] = 'O';
                        break;
                    }
                }
                else
                    break;
            }
        }

        if (str.find('s') != str.npos)
        {
            arr[pl.oY][pl.oX] = ' ';
            for(;;)
            {
                if(pl.oY < 22)
                {
                    ++pl.oY;
                    if(arr[pl.oY][pl.oX] == ' ' || arr[pl.oY][pl.oX] == '*')
                    {
                        if (arr[pl.oY][pl.oX] == '*')
                        {
                            food.life = false;
                            score += 25;
                        }
                        arr[pl.oY][pl.oX] = 'O';
                        break;
                    }
                    else
                    {
                        --pl.oY;
                        arr[pl.oY][pl.oX] = 'O';
                        break;
                    }
                }
                else
                    break;
            }
        }
    }
}


int main()
{
  srand(static_cast<unsigned int>(time(0)));
  rand();


  for(int lines = 0; lines < 23; ++lines)
  {
    if(lines % 22 == 0)
      arr.push_back("##############################");
    else
      arr.push_back("#                            #");
  }

  std::thread(moving).detach();

  for(;;)
  {
    if (!food.life)
    { 
      destroy_walls();
      create_player();
      create_food();
      create_walls();
      food.life = true;
    }

    std::cout << '\n';
    for(auto it : arr)
      std::cout << "   " << it << '\n';
    std::cout << "   Playr score:"<<score<<'\n';
    wait(1500);
    system("clear");
  }

  return 0;
}
