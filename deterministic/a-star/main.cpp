    #include <stdlib.h>
    #include <iostream>
    #include <stdio.h>
    #include <sys/socket.h>
    #include <netinet/in.h>
    #include <string.h>
    #include <arpa/inet.h>
    #include <unistd.h>
    #include"probcal.hpp"
    #include <chrono>
    #include <fstream>

    #define PORT 4000
    using namespace std;
    using namespace chrono;

    struct sockaddr_in address;
    struct sockaddr_in serv_addr;

    struct lenk
    {
        int id;
        int f;
        int g;
        int h;
        int x;
        int y;
        lenk* next;
        lenk* prev;
        lenk* parent;

    };
    class linklist
    {
        private:
            lenk * first;
            lenk * end;

        public:

            int count;
            linklist()
            {
                first = NULL;
                end =NULL;
                count=0;
            }

            lenk* get (int id)
            {
                lenk* current=first;
                while (current!=NULL) {
                    if (current->id == id) {
                        return current;
                    }
                    current = current->next;
                    break;
                }
            }

            void getpath(int path[][3])
            {
                int c=0;
                lenk* current=first;
                path[0][0]=c;
                while(current !=NULL )
                {
                    path[0][0]= ++c ;
                    path[c][0]=current->x;
                    path[c][1]=current->y;

                    //cout<<path[c][0]<<"\t"<<path[c][1]<<endl;
                    current=current->parent;
                }

            }

            void check1 (int child[][5])   //open list 3
            {
                for (int i = 1; i <=child[0][0] ; ++i) {
                    lenk* current = first;
                    while (current!=NULL)
                    {
                        if( child[i][0]==current->x  && child[i][1]==current->y &&  current->f < child[i][2] )
                            child[i][3]=0;
                        else
                            child[i][3]=1;

                        current=current->next;
                    }

                }

            }

            void check2 (int child[][5])   //closed list 4
            {
                for (int i = 1; i <=child[0][0] ; ++i) {
                    lenk* current = first;
                    while (current!=NULL)
                    {
                        if( child[i][0]==current->x  && child[i][1]==current->y &&  current->f <child[i][2] )
                            child[i][4]=0;
                        else
                            child[i][4]=1;

                        current=current->next;
                    }

                }

            }

            void additem(int g,int h,int x,int y,lenk* p)
            {
                count++;

                if ( first == NULL && end==NULL )
                {
                    lenk* newlink = new lenk;
                    newlink->id= count;
                    newlink->x = x;
                    newlink->y = y;
                    newlink->g = g;
                    newlink->h = h;
                    newlink->parent=p;
                    newlink->f=g+h;
                    newlink->next = NULL;
                    newlink->prev=NULL;
                    first = newlink;
                    end=newlink;
                }
                else
                {
                    int d=g+h;
                    if (d <= first->f )
                    {
                        lenk* newlink = new lenk;
                        newlink->id= count;
                        newlink->g = g;
                        newlink->h = h;
                        newlink->parent=p;
                        newlink->x = x;
                        newlink->y = y;
                        newlink->f=g+h;
                        newlink->next=first;
                        newlink->prev=NULL;

                        first->prev=newlink;

                        first =newlink;
                    }
                    else if ( d >= end->f )
                    {
                        lenk* newlink = new lenk;
                        newlink->id= count;
                        newlink->g = g;
                        newlink->h = h;
                        newlink->parent=p;
                        newlink->x = x;
                        newlink->y = y;
                        newlink->f=g+h;
                        newlink->next=NULL;
                        newlink->prev=end;
                        end->next=newlink;
                        end=newlink;
                    }
                    else
                    {
                        lenk* current = first;
                        while( current != NULL )
                        {
                            if( d<current->f || (d==current->f && g<=current->g))
                            {
                                lenk* newlink = new lenk;
                                newlink->id= count;
                                newlink->g = g;
                                newlink->h = h;
                                newlink->parent=p;
                                newlink->x = x;
                                newlink->y = y;
                                newlink->f=g+h;
                                newlink->next=current;
                                newlink->prev=current->prev;
                                current->prev->next=newlink;
                                current->prev=newlink;
                                break;
                            }
                            if( d==current->f && g>current->g)
                            {
                                current=current->next;
                                lenk* newlink = new lenk;
                                newlink->id= count;
                                newlink->g = g;
                                newlink->parent=p;
                                newlink->x = x;
                                newlink->y = y;
                                newlink->h = h;
                                newlink->f=g+h;
                                newlink->next=current;
                                newlink->prev=current->prev;
                                current->prev->next=newlink;
                                current->prev=newlink;
                                break;
                            }
                            current = current->next;
                        }

                    }

                }
                arrange();
            }
            void display()
            {
                lenk* current = first;
                while( current != NULL )
                {
                    cout << current->id<<"\t"<< current->f<<"\t" <<current->x <<"\t"<<current->y <<endl;
                    current = current->next;
                }
            }

            void arrange()
            {
                lenk* current = first;
                for(int i=0 ; i<count ; i++)
                {
                    current->id=i+1;
                    current = current->next;
                }
            }

            void remove(int i)
            {
                if (count==1)
                {
                    first=end=NULL;
                }
                else if (i==1)
                {
                    first=first->next;
                    first->prev=NULL;
                }
                else if (i==count)
                {
                    end=end->prev;
                    end->next=NULL;
                }
                else
                {
                    lenk* current = first;
                    while( current != NULL )
                    {
                        if(i==current->id)
                        {
                            current->prev->next=current->next;
                            current->next->prev=current->prev;
                            break;
                        }
                        current = current->next;
                    }

                }
                count--;
                this->arrange();
            }
    };


    void display(int path[][3],char path_letter[],int startx, int starty,int goalx,int goaly,int staticop[][2],int dynamicop[][2],int sizex,
                 int sizey,int n_of_st,int n_of_dy);

    void sethuristic (int** huristic,int goalx, int goaly,int staticop[][2],int n_of_st, int dynamicop[][2],int n_of_dy,int sizex, int sizey);

    int dist(int sx,int sy,int gx , int gy);
    void genrate (int px, int py ,int child[][5],int sizex,int sizey);
    void path_convereter(int path[][3],char tmp[]);

    void a_star (int startx, int starty,int goalx,int goaly,int staticop[][2],int dynamicop[][2],int sizex,
                 int sizey,int path[][3],int n_of_st,int n_of_dy);



    void initialize_connection (int & sock);

    int main()
    {

        int startx , starty , goalx , goaly;
        int sizex;
        int sizey;
        int n_of_st;
        int n_of_dy;
        int count=0,c=0;

        int path[100][3];

        long ms=0.0 ;

        int sock = 0;
        char msg[1024] = "start";
        char buffer[1024] = {0};


        initialize_connection (sock);

        send(sock,  msg,  strlen(msg),  0 );
        printf("hello let's start playing \n" );
        read( sock,  buffer,  1024);
        printf("%s\n", buffer );

        cout<<"======================================================"<<endl;

        stringstream stream(buffer);

        int n;
        stream>>n;
        sizex=n;
        stream>>n;
        sizey=n;
        stream>>n;
        n_of_st=n;
        stream>>n;
        n_of_dy=n;
        stream>>n;
        starty=n;
        stream>>n;
        startx=n;


        int staticop[n_of_st][2];
        int dynamicop[n_of_dy][2];

        for (int j =0 ; j <n_of_dy; ++j)
        {
            stream>>n;
            dynamicop[j][1]=n;
            stream>>n;
            dynamicop[j][0]=n;
        }
        for (int j =0 ; j <n_of_st; ++j)
        {
            stream>>n;
            staticop[j][1]=n;
            stream>>n;
            staticop[j][0]=n;
        }
        stream>>n;
        goaly=n;
        stream>>n;
        goalx=n;




        while (true)
        {
            count++;
            c=0;
            path[0][0]=0;
            cout<<"start position==> "<<startx<<"\t"<<starty<<endl;

            auto t1 = high_resolution_clock::now();

            a_star(startx, starty, goalx, goaly, staticop, dynamicop, sizex, sizey, path, n_of_st, n_of_dy);
            char path_letter[path[0][0]];
            path_convereter(path, path_letter);
            //display(path,path_letter,startx,starty,goalx,goaly,staticop,dynamicop,sizex,sizey,n_of_st,n_of_dy);

            for (int i = 0; i < strlen(path_letter) - 1; i++) {
                msg[c++] = path_letter[i];
                msg[c++] = ' ';
            }
            auto t2 = high_resolution_clock::now();
            auto diff = duration_cast<duration<double>>(t2 - t1);
            ms += (long)(1000*diff.count());

            send(sock, msg,1, 0);
            cout<<"sending action "<<msg[0]<<endl;
            memset(buffer,0,sizeof(buffer));
            read(sock, buffer, 1024);
            printf("Update Message ==>> %s\n", buffer);

            stringstream stream(buffer);
            stream>>n;
            starty=n;
            stream>>n;
            startx=n;

            switch (msg[0]) {
                case '1' :
                    startx--;
                    break;
                case '2':
                    startx++;
                    break;
                case '3':
                    starty++;
                    break;
                case '4':
                    starty--;
                    break;
            }

            if (startx==goalx && starty==goaly)
            {
                cout<<"======================================================"<<endl;
                msg[0]='0';
                send(sock, msg,1, 0);
                printf("Final state ====> ");
                memset(buffer,0,sizeof(buffer));
                read(sock, buffer, 1024);
                printf("%s\n", buffer);
                break;
            }

            if (buffer[0]=='l' || buffer[0]=='w')
                break;

            for (int j =0 ; j <n_of_dy; ++j)
            {
                stream>>n;
                dynamicop[j][1]=n;
                stream>>n;
                dynamicop[j][0]=n;
            }
            cout<<"======================================================"<<endl;
        }

        cout<<"DONE with Time = "<<ms<<endl;

        ofstream result;
        result.open ("output.txt");
        result <<buffer<<endl;
        result <<ms;
        result.close();


        //display(path,path_letter,startx,starty,goalx,goaly,staticop,dynamicop,sizex,sizey,n_of_st,n_of_dy);

        return 0;
    }
    void initialize_connection (int & sock)
    {


        if ((sock =  socket(AF_INET, SOCK_STREAM,  0)) <  0)
        {
            printf("\n Socket creation error \n");
            exit(0);
        }

        memset( & serv_addr, '0',  sizeof(serv_addr));

        serv_addr.sin_family =  AF_INET;
        serv_addr.sin_port =  htons(PORT);

        //  Convert IPv4 and IPv6 addresses from text to binary form
        if(inet_pton(AF_INET,  "127.0.0.1",& serv_addr.sin_addr) <= 0)
        {
            printf("\nInvalid address / Address not supported \n");
            exit(0);
        }

        if (connect(sock,  (struct sockaddr *) & serv_addr,  sizeof(serv_addr)) <  0)
        {
            printf("\nConnection Failed \n");
            exit(0);
        }
    }

    void a_star (int startx, int starty,int goalx,int goaly,int staticop[][2],int dynamicop[][2],int sizex,
                 int sizey,int path[][3],int n_of_st,int n_of_dy)
    {
        bool solved= 0;
        int child[5][5];
        linklist openlist,closedlist;
        int **huristic;

        huristic= new int*[sizex];
        for(int i = 0; i < sizex; i++)
            huristic[i] = new int[sizey];


        sethuristic(huristic,goalx,goaly,staticop,n_of_st,dynamicop,n_of_dy,sizex,sizey);

        openlist.additem(0,huristic[startx][starty],startx,starty,NULL);


        while (openlist.count>0 && !solved)
        {
            openlist.arrange();
            lenk* node=openlist.get(1);
            openlist.remove(1);
            genrate(node->x,node->y,child,sizex,sizey);

            for (int i = 1; i <= child[0][0] ; ++i)
            {

                if(child[i][0]==goalx && child[i][1]==goaly)
                {
                    //cout<<"Goal reached \n";
                    solved=1;
                    break;
                }

                child[i][2]=node->g+1+huristic[child[i][0]][child[i][1]];
                openlist.check1(child);
                closedlist.check2(child);
            }
            for (int i = 1; i <= child[0][0] ; ++i)
            {
                if(child[i][3]==1 && child[i][4]==1)
                    openlist.additem(node->g+1,huristic[child[i][0]][child[i][1]],child[i][0],child[i][1],node);
            }
            closedlist.additem(node->g,node->h,node->x,node->y,node->parent);
            /*cout <<"openlist \n";
            openlist.display();

            cout <<"closedlist \n";
            closedlist.display();*/
        }
    openlist.getpath(path);
    }

    void path_convereter(int path[][3],char tmp[])
    {
        int count=-1;

        for (int i = path[0][0]; i >0; --i)
        {
            count++;
            if(path[i][0]==path[i-1][0] && path[i-1][1]-path[i][1]>0)
                tmp[count]='3';
            if(path[i][0]==path[i-1][0] && path[i-1][1]-path[i][1]<0)
                tmp[count]='4';
            if(path[i][1]==path[i-1][1] && path[i-1][0]-path[i][0]>0)
                tmp[count]='2';
            if(path[i][1]==path[i-1][1] && path[i-1][0]-path[i][0]<0)
                tmp[count]='1';
        }
        tmp[count]='G';
    }

    void genrate (int px, int py ,int child[][5],int sizex,int sizey)
    {
        int id;
        for (int i = 0; i <5 ; ++i)
            for (int j = 0; j <5 ; ++j)
                child[i][j]=1;

        child[0][0]=0;
        if(px+1>=0 && px+1 <=sizex-1) {
            id=++child[0][0];
            child[id][0] = px + 1;
            child[id][1] = py;
        }

        if(px-1>=0 && px-1 <=sizex-1) {
            id=++child[0][0];
            child[id][0] = px - 1;
            child[id][1] = py;
        }
        if(py+1>=0 && py+1 <=sizey-1) {
            id=++child[0][0];
            child[id][0] = px;
            child[id][1] = py+1;
        }
        if(py-1>=0 && py-1 <=sizey-1) {
            id=++child[0][0];
            child[id][0] = px;
            child[id][1] = py-1;
        }
    }
    void sethuristic (int** huristic,int goalx, int goaly,int staticop[][2],int n_of_st,int dynamicop[][2],int n_of_dy,int sizex, int sizey)
    {
        int factor=15;
        int histcount=3;
        int step=1;
        int total =sizex*sizey;
        int hist[1][2];
        double** temp;
        double** prob;
        double** huristic_prob;
        int c=0;

        huristic_prob = new double*[sizex];
        for(int i = 0; i < sizex; i++)
            huristic_prob[i] = new double[sizey];

        prob = new double*[total];
        for(int i = 0; i < total; i++)
            prob[i] = new double[histcount];

        for (int l = 0; l <total; ++l)
            for (int i = 0; i <histcount; ++i)
                prob[l][i]=0;

        temp = new double*[total];
        for(int i = 0; i < total; i++)
            temp[i] = new double[histcount];


        for (int i = 0; i <sizex ; ++i) {
            for (int j = 0; j <sizey; ++j) {
                huristic[i][j]=dist(i,j,goalx,goaly);
            }
        }

        for (int m = 0; m <n_of_dy; ++m)
        {
            for (int l = 0; l <total; ++l)
                for (int i = 0; i <histcount; ++i)
                    temp[l][i]=0;

            hist[0][0]=dynamicop[m][0];
            hist[0][1]=dynamicop[m][1];
            getProb(histcount,hist,sizex,sizey,temp,step);

            for (int j = 0; j <total ; ++j)
            {
                prob[j][0]+=temp[j][0];
            }
        }

        for (int k = 0; k <n_of_st; ++k) {
            huristic[staticop[k][0]][staticop[k][1]]=1000;
        }


        for (int l = 0; l <sizex; ++l) {
            for (int i = 0; i <sizey; ++i)  // put histcount when need to use
            {
                huristic_prob[l][i]=prob[c++][0]*factor;
                huristic[l][i]+=huristic_prob[l][i];
            }
        }

        //cout<<"======================================================"<<endl;

        /*for (int l = 0; l <sizex; ++l) {
            for (int i = 0; i <sizey; ++i)
            {
                cout<<huristic_prob[l][i]<<"\t";
            }
            cout<<endl;
        }*/

        //cout<<"======================================================"<<endl;

        huristic[goalx][goaly]=0;

       /* cout << "huristic value \n";
        for (int i = 0; i < sizex; ++i) {
            for (int j = 0; j < sizey ; ++j) {
                cout<<huristic[i][j]<<"\t";
             }
            cout<<endl;
        }*/

    }

    int dist(int sx,int sy,int gx , int gy)
    {
        return abs(gy-sy)+abs(gx-sx);
    }

    void display(int path[][3],char path_letter[],int startx, int starty,int goalx,int goaly,int staticop[][2],int dynamicop[][2],int sizex,
                 int sizey,int n_of_st,int n_of_dy)
    {
        char map[sizex][sizey];

        for (int i = 0; i <sizex; ++i)
            for (int j = 0; j <sizey; ++j)
                map[i][j]='-';

        for (int i =path[0][0]; i >0 ; --i)
            map[path[i][0]][path[i][1]]='*';

        for (int i =0; i <n_of_st ; ++i)
            map[staticop[i][0]][staticop[i][1]]='#';

        for (int i =0; i <n_of_dy ; ++i)
            map[dynamicop[i][0]][dynamicop[i][1]]='#';

        map[startx][starty]='S';
        map[goalx][goaly]='G';

        cout<<"Path from the start to goal \n";

       /* for (int i = 0; i <sizex; ++i) {
            for (int j = 0; j < sizey; ++j)
                cout << map[i][j] << "\t";
            cout << endl;
        }*/


        cout<<"Full path in letters ---->";
        for (int j = 0; j < path[0][0]; ++j)
            cout<<path_letter[j];



        for (int i = 0; i <sizex; ++i)
            for (int j = 0; j <sizey; ++j)
                map[i][j]='-';
        map[startx][starty]='S';
        map[goalx][goaly]='G';

        int t=0;
        for (int i =path[0][0]; i >0 ; --i)
        {
            map[path[i][0]][path[i][1]] = path_letter[t];
            t++;
        }
        for (int i =0; i <n_of_st ; ++i)
            map[staticop[i][0]][staticop[i][1]]='#';

        for (int i =0; i <n_of_dy ; ++i)
            map[dynamicop[i][0]][dynamicop[i][1]]='#';



        cout<<"\npath from the start to goal wirh direcions \n";

        for (int i = 0; i <sizex; ++i) {
            for (int j = 0; j < sizey; ++j)
                cout << map[i][j] << "\t";
            cout << endl;
        }
    }
