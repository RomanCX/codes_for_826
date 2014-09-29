int DRtreeRingSearch(
	int	rfd,		/* DR index file descriptor */
	int	rootpage,	/* root node page number */

	DRrect	*outerrect,	/* outer rectangle */
	DRrect  *innerrect, /* inner rectangle */
	bool_t	(*testfn)(),	/* comparison function */
	int	(*outputfn)())	/* rect output function */
{
	int	errno;		/* error number */
	char	*pagebuf;	/* pointer to page buffer of node */
	int	count;		/* number of records found */
	int	i;		/* entry number */

	/*--------------------------------------------------------------------*/
	/* check "rfd" */
	if (DRfdInvalid(rfd))
		return DRerrSave(DRE_RFDINVALID);

	/* check "testrect" */
	if (DRrectInvalid(rfd, testrect))
		return DRerrSave(DRE_RECTINVALID);

	/* get the root node page */
	if (PS_GetThisPage(DRftab[rfd].pfd, rootpage, &pagebuf) != PSE_OK)
		return DRerrSave(DRE_PSERROR);

	/* initialize "count" and "errno" */
	errno = count = 0;

	/*--------------------------------------------------------------------*/
	/* if the node is NOT a leaf... */
	if (DRnodeLevel(rfd, pagebuf) > 0)
	{
		/* for each entry in the node... */
		for (i = DRnodeFirstEntry(rfd, pagebuf);
			i >= 0 && i < DRnodeCard(rfd);
			i = DRnodeNextEntry(rfd, i, pagebuf))
		{
			/* if "testrect" matches the rect of an entry... */
			if ((*testfn)(rfd, outerrect, innerrect,
				DRnodeRect(rfd, i, pagebuf)))
			{
				/* search the child of the entry */
				errno = DRtreeSearchEntry(rfd,
						DRnodeChild(rfd, i, pagebuf),
						outerrect, innerrect, testfn, outputfn);

				/* if bad things happened... */
				if (errno < 0)
					/* stop searching */
					break;

				/* increase counter */
				count += errno;
			}
		}
	}
	/*--------------------------------------------------------------------*/
	/* if the node IS a leaf... */
	else if (DRnodeLevel(rfd, pagebuf) == 0)
	{
		/* for each entry in the node... */
		for (i = DRnodeFirstEntry(rfd, pagebuf);
			i >= 0 && i < DRnodeCard(rfd);
			i = DRnodeNextEntry(rfd, i, pagebuf))
		{
			/* if "testrect" matches the rect of an entry... */
			if ((*testfn)(rfd, outerrect, innerrect,
				DRnodeRect(rfd, i, pagebuf)))
			{
				/* output the entry */
				(*outputfn)(rfd, i, pagebuf);

				/* increase counter */
				count++;
			}
		}
	}
	/*--------------------------------------------------------------------*/
	/* the level info in the root node is corrupted */
	else
		errno = DRerrSave(DRE_LEVELINVALID);

	/*--------------------------------------------------------------------*/
	/* unfix the root node page */
	if (PS_UnfixPage(DRftab[rfd].pfd, rootpage, FALSE) != PSE_OK)
		return DRerrSave(DRE_PSERROR);

	/* return error number if there is any */
	if (errno < 0)
		return errno;

	/* return the number of entries found */
	return count;
}
