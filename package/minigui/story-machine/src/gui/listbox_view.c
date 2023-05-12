#include "listbox_view.h"
#include <sys/time.h>

void ListBoxDataDeinit(ListboxView *listboxView)
{
	if(listboxView != NULL){
		if(listboxView->itDt != NULL){
			if(listboxView->itDt->btn != NULL){
				unloadBitMap(&listboxView->itDt->btn->offBmp);
				unloadBitMap(&listboxView->itDt->btn->onBmp);
				free(listboxView->itDt->btn);
				listboxView->itDt->btn = NULL;
			}
			unloadBitMap(&listboxView->itDt->rB);
			free(listboxView->itDt);
			listboxView->itDt = NULL;
		}
		unloadBitMap(&listboxView->winBg);
		unloadBitMap(&listboxView->ltbBg);
		free(listboxView);
	}
}


static int ListboxViewProc(HWND hwnd, int message, WPARAM wParam,LPARAM lParam)
{
	int i;
	int newX = 0;
	int newY = 0;
	int newItem = 0;
	int selectIndex = -1;	// -1 is invalid
	ListboxView *list = NULL;
	list = (ListboxView*)GetWindowAdditionalData(hwnd);

	switch (message)
	{
	case MSG_CREATE:
	{
		gui_debug("MSG_CREATE\n");
		return 0;
	}
	case MSG_ERASEBKGND: {
		gui_debug("MSG_ERASEBKGND\n");
		return 0;
	}
	case MSG_PAINT:
	{
		gui_debug("MSG_PAINT\n");
		int res;
		int a, b;
		SIZE size;
		HDC hdc = BeginPaint(hwnd);

		long long total_time;
		struct timeval start, end;

		gettimeofday(&start, NULL);
		gui_debug("%d %d\n",list->moveGap,list->Index);

		// calculator the index
		a = list->moveGap / (list->iy+ list->vgap);
		b = list->moveGap % (list->iy+ list->vgap);
		list->moveGap = b;
		list->Index = list->Index - a;
		if(list->Index <= 0)
		{
			list->Index = 0;
			if(list->moveGap>=0)
			{
				list->moveGap = 0;
			}
		}
		else if(list->Index >= list->itSz-list->showSz)
		{
			list->Index = list->itSz-list->showSz;
			if(list->moveGap<0)
			{
				list->moveGap = 0;
			}
		}

		SetBkMode(hdc, BM_TRANSPARENT);

		// if index more than 0, more draw item
		if(list->Index > 0)
		{
			newItem=-1;
		}
		else
		{
			newItem=0;
		}
#if 0
		if(list->winBg.bmBits){
			FillBoxWithBitmap(hdc, 0, 0, list->winBg.bmWidth, list->winBg.bmHeight, &list->winBg);
		}
#endif
		#if 0
		for(; newItem<=list->showSz; newItem++)
		{
			if(list->ltbBg.bmBits){
					FillBoxWithBitmap(hdc, list->hfp, (list->iy+ list->vgap) * newItem + list->moveGap, list->ix, list->iy, &list->ltbBg);
				}

			// draw item left thing
			gui_debug("index : [%d]\n", list->Index);
			GetTextExtent(hdc, list->itDt[list->Index+newItem].lT, -1, &size);
			TextOut(hdc, list->hfp + list->lthGap,
				(list->iy-size.cy) / 2 + (list->iy+ list->vgap) * newItem + list->moveGap, list->itDt[list->Index+newItem].lT);
		}
		#endif

		#if 1
		// draw the item

		for(; newItem < list->showSz + 1; newItem++)
		{
			if((newItem + list->Index) >= list->itSz)
			{
				break;
			}

			SetTextColor(hdc, list->lTextColor);
			SelectFont(hdc, list->lTextFt[list->Index+newItem]);

			// draw item mode 2
			if((list->itType & ITEM_BMP) && (list->itType & ITEM_LINE))
			{
				if(list->ltbBg.bmBits){
					FillBoxWithBitmap(hdc, list->hfp, (list->iy+ list->vgap) * newItem + list->moveGap, list->ix, list->iy, &list->ltbBg);
				}

				SetPenColor(hdc, list->lineColor);
				MoveTo(hdc, list->hfp, (list->iy + list->vgap) * newItem - (list->vgap/2) + list->moveGap);
				LineTo(hdc, list->hfp + list->ix, (list->iy + list->vgap) * newItem - (list->vgap/2) + list->moveGap);
			}
			else if(list->itType & ITEM_BMP)
			{
				if(list->ltbBg.bmBits){
					FillBoxWithBitmap(hdc, list->hfp, (list->iy+ list->vgap) * newItem + list->moveGap, list->ix, list->iy, &list->ltbBg);
				}
			}
			else if(list->itType & ITEM_LINE)
			{
				// draw item line
				SetPenColor(hdc, list->lineColor);
				MoveTo(hdc, list->hfp, (list->iy + list->vgap) * newItem - (list->vgap/2) + list->moveGap);
				LineTo(hdc, list->hfp + list->ix, (list->iy + list->vgap) * newItem - (list->vgap/2) + list->moveGap);
			}

			// draw item left thing
			//gui_debug("index : [%d]\n", list->Index);

			//draw item right thing

			if(list->itDt[list->Index+newItem].rType & RITEM_LTEXT){
				GetTextExtent(hdc, list->itDt[list->Index+newItem].lT, -1, &size);
				SetPenColor(hdc, list->lineColor);
				TextOut(hdc, list->hfp + list->lthGap,
					(list->iy-size.cy) / 2 + (list->iy+ list->vgap) * newItem + list->moveGap,
				list->itDt[list->Index+newItem].lT);
			}
			else if(list->itDt[list->Index+newItem].rType & RITEM_RTEXT)
			{
				GetTextExtent(hdc, list->itDt[list->Index+newItem].rT, -1, &size);
				TextOut(hdc, list->hfp + list->ix - list->lthGap - size.cx,
					(list->iy-size.cy) / 2 + (list->iy+ list->vgap) * newItem + list->moveGap, list->itDt[list->Index+newItem].rT);
			}
			else if(list->itDt[list->Index+newItem].rType & RITEM_BMP)
			{
				if(list->itDt[list->Index+newItem].rB.bmBits){
					FillBoxWithBitmap(hdc, list->hfp + list->ix - list->lthGap - list->itDt[list->Index+newItem].rB.bmWidth,
						(list->iy-list->itDt[list->Index+newItem].rB.bmHeight) / 2 + (list->iy+ list->vgap) * newItem + list->moveGap,
						list->itDt[list->Index+newItem].rB.bmWidth,
						list->itDt[list->Index+newItem].rB.bmHeight, &list->itDt[list->Index+newItem].rB);
					}
			}
			else if(list->itDt[list->Index+newItem].rType & RITEM_BUTTON)
			{
				if(list->itDt[list->Index+newItem].btn->onStus)
				{
					if(list->itDt[list->Index+newItem].btn->onBmp.bmBits){
						FillBoxWithBitmap(hdc, list->hfp + list->ix - list->lthGap - list->itDt[list->Index+newItem].btn->onBmp.bmWidth,
						(list->iy-list->itDt[list->Index+newItem].btn->onBmp.bmHeight) / 2 + (list->iy+ list->vgap) * newItem + list->moveGap,
						list->itDt[list->Index+newItem].btn->onBmp.bmWidth,
						list->itDt[list->Index+newItem].btn->onBmp.bmHeight, &list->itDt[list->Index+newItem].btn->onBmp);
					}
				}
				else
				{
					if(list->itDt[list->Index+newItem].btn->offBmp.bmBits){
						FillBoxWithBitmap(hdc, list->hfp + list->ix - list->lthGap - list->itDt[list->Index+newItem].btn->offBmp.bmWidth,
						(list->iy-list->itDt[list->Index+newItem].btn->offBmp.bmHeight) / 2 + (list->iy+ list->vgap) * newItem + list->moveGap,
						list->itDt[list->Index+newItem].btn->offBmp.bmWidth,
						list->itDt[list->Index+newItem].btn->offBmp.bmHeight, &list->itDt[list->Index+newItem].btn->offBmp);
					}
				}

				if(list->itDt[list->Index+newItem].btn->sltItem >= 0)
				{
					SelectFont(hdc, list->rTextFt);
					//SetTextColor(hdc, PIXEL_lightgray);
					SetTextColor(hdc, list->rTextColor);
					GetTextExtent(hdc, list->itDt[list->Index+newItem].btn->text, -1, &size);
					TextOut(hdc, list->hfp + list->ix - list->lthGap - size.cx - list->itDt[list->Index+newItem].btn->onBmp.bmWidth,
						(list->iy-size.cy) / 2 + (list->iy+ list->vgap) * newItem + list->moveGap,
						list->itDt[list->Index+newItem].btn->text);
				}
			}


		}
		#endif

		EndPaint(hwnd, hdc);

		gettimeofday(&end, NULL);
		total_time = (end.tv_sec - start.tv_sec) * 1000000 + (end.tv_usec - start.tv_usec);
		total_time /= 1000;
		gui_debug("time %lld ms\n", total_time);

		return 0;
	}

	case MSG_LBUTTONDOWN:
	{
		newX = LOWORD(lParam);
		newY = HIWORD(lParam);
		gui_debug("MSG_LBUTTONDOWN\n");
		gui_debug("newX=%d, newY=%d\n", newX, newY);
		list->isSlidText = TRUE;
		list->oldY = newY;
		list->moveNum = 0;

		//InvalidateRect(list->sfHwnd, NULL, TRUE);
		break;
	}

	case MSG_LBUTTONUP:
	{
		gui_debug("MSG_LBUTTONUP\n");
		newX = LOWORD(lParam);
		newY = HIWORD(lParam);
		gui_debug("newX=%d, newY=%d\n", newX, newY);
		list->isSlidText = FALSE;

		if(list->Index <= 0)
		{
			list->moveGap = 0;
		}
		if(list->Index >= list->itSz-list->showSz)
		{
			list->moveGap = 0;
		}

		// calculator item
		if(list->Index > 0)
		{
			newItem=-1;
		}
		else
		{
			newItem=0;
		}

		for(; newItem<=list->showSz; newItem++)
		{
			if(newY > ((list->iy+ list->vgap) * newItem + list->moveGap)
				&& newY < ((list->iy+ list->vgap) * newItem + list->moveGap + list->iy))
			{
				selectIndex = list->Index + newItem;
			}
		}

		if(list->moveFlag == 1)
		{
			list->moveFlag = 0;
			selectIndex = -1;
		}
		gui_debug("index=%d, selectIndex=%d\n", list->Index, selectIndex);

		if(selectIndex >= 0)
		{
			if(list->itDt[selectIndex].rType & RITEM_BUTTON)
			{
				if(list->itDt[selectIndex].btn->onStus)
				{
					list->itDt[selectIndex].btn->onStus = 0;
				}
				else
				{
					list->itDt[selectIndex].btn->onStus = 1;
				}
				//gui_debug("index=%d, selectIndex=%d\n", list->Index, selectIndex);
				list->itDt[selectIndex].btn->fun(selectIndex, list);
				//gui_debug("index=%d, selectIndex=%d\n", list->Index, selectIndex);
				selectIndex = -1;
			}
		}

		InvalidateRect(list->sfHwnd, NULL, TRUE);

		//SetTimer(hwnd, LISTBOX_TIMER_ID, 1);
		break;
	}

	case MSG_MOUSEMOVE:
	{
		newX = LOWORD(lParam);
		newY = HIWORD(lParam);

		if (list->isSlidText) {
			list->moveNum++;
			if (list->moveNum >= 2) {
				list->moveNum = 0;
				int gap = newY - list->oldY;
				int newGap = list->moveGap + gap;
				list->moveFlag = 1;
				if ((list->Index <= 0 && gap > 0) || (list->Index >= list->itSz-list->showSz && gap < 0))
				{
					break;
				}
				gui_debug("moveinfo: (%d, %d, %d, %d, %d, %d)\n", newX, newY, list->oldY, gap, list->moveGap, list->Index);
				list->oldY = newY;
				list->moveGap = newGap;

				InvalidateRect(list->sfHwnd, NULL, TRUE);
			}
		}
		break;
	}

	default:
		break;
	}

	return DefaultControlProc(hwnd, message, wParam, lParam);
}


BOOL RegisterListboxView(void)
{
	WNDCLASS MyClass;
	MyClass.spClassName = LISTBOX_VIEW;
	MyClass.dwStyle = WS_NONE;
	MyClass.dwExStyle = WS_EX_NONE;
	MyClass.hCursor = 0;
	MyClass.iBkColor = PIXEL_lightgray;
	MyClass.WinProc = ListboxViewProc;

	return RegisterWindowClass(&MyClass);
}

void UnregisterListboxView(void)
{
	UnregisterWindowClass(LISTBOX_VIEW);
}
