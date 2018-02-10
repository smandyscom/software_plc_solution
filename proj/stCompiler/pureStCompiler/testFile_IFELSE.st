/******************************************************************/
/* THIS IS FILE BUILD FOR TESTING IF_ELSE PAIR  , CODE GENERATING */
/*	Hsien , 2013.02.04											  */
/******************************************************************/

/*******************/
/* Multi-If-Branch */
/*******************/
if( RC_R1 == 1 & RC_R2 == 1){
	Y2.01 = 1;
}
end

if( RC_R1 == 2 )
	Y2.03 = 1;
end

/*************/
/* Nested IF */
/*************/
if(RC_R1 == 1)
	if(RC_R1 == 2)
		Y2.01 = 1;
	end
end

/****************/
/* IF-ELSE pair */
/****************/
if(RC_R1 == 1)
	Y2.01 = 1;
else
	Y2.02 = 1;
end

/**********************/
/* Multi-IF-ELSE pair */
/**********************/
if(RC_R1 == 1)
	Y2.01 = 1;
else
	Y2.02 = 1;
end

/* above NEST , BRANCH tested OK , Hsien , 2013.02.04*/

/**********************/
/*	Nested IF-IF-ELSE */
/**********************/
if(RC_R1 == 1)
	if(RC_R1 == 2)
		Y2.01 = 1;
	else
		Y2.02 = 1;
	end
end
/* test OK , Hsien , 2013.02.04*/

/*************************/
/* Nested IF-ELSE-IF-ELSE*/
/*************************/
if(RC_R1 == 1)
	if(RC_R1 == 2)
		Y2.01 = 1;
	else
		Y2.02 = 1;
	end
else
	Y2.03 = 1;
end	
/* nested if-else tested OK  ,Hsien , 2013.02.04*/

/* When Condition is Assignment Expression */
if(RC_R1 = 1)
	Y2.01 = 1;
end
/* after adding a intermedium grammar 'ifcondition' , multi-assignment and if(assignment) are 
	co-worked , Hsien , 2013.02.04*/

/********************/
/* Multi Assignment */
/********************/
Y2.01 = Y2.03 = Y2.04; /* tested ok  , Hsien , 2013.02.04*/
