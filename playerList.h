#ifndef PLAYERLIST_H
#define PLAYERLIST_H


//#include <stdio.h>
#define NAMELEN 40


typedef struct _listStuff {
                       char name[NAMELEN+1];
                       int kills;
                       int deaths;
					   int caps;
                       double percent;
                       int rank;
					   int points;
					   long dateInSec;
					   int suicides;
		      		   _listStuff *next;
                       } _listStuff;


class playerList 
   {

   public:

				 playerList(void);
				 ~playerList(void);
	  void		 operator=(playerList *lptr);
      _listStuff *findName(const char *name);
      void		 insertName(const char *name, int win, int loss, int capture, long dateSec, int suicide); 
      void		 deleteName(const char *name);
      int		 getNumNames();
	  char		 *getListDate() {return _listDate;}
      void		 updateList(char *name, int death, int kill, int cap, int suicide);
      void		 dump();
	  void		 deletePlayerFile(char *fileName);
	  void		 processPlayerList(const char *outfile);
	  void		 readList(char *readFileName, int retention);
	  void		 setToTopOfList() {_curListPos = pList;}
	  int        getNextPlayer();
	  char       *getName();
	  int        getKills();
	  int        getDeaths();
	  int		 getCaps();
	  int		 getRank();
	  double	 getPercent();
	  int		 getPoints();
	  int		 getSuicides();


   private:

      char			_listDate[DATELEN+1];
      int           _curNumNames;
	  _listStuff    *pList;
      _listStuff    *_curListPos;
	  void			swapStuff(_listStuff *p1, _listStuff *p2,_listStuff *prev);
	  long			getDateInSec();
	  int			needToDelete(long, long, int);
//	  void			sortListByPercent();
	  void			sortListByKills();
	  void			sortListByPoints();
      void			calculatePercent();
	  void			calculatePoints();
	  void			updateRank();
	  void			writePlayerFile(const char *outputFile);
		
   };

#endif
