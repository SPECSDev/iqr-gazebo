


skip = 2;
m=importdata('ambiguous.dat',';',skip);

x=m.data(:,2);
y=m.data(:,3);

indexT=find(diff(y)<-2);
indexT=[indexT(1:(end-1))+40,indexT(2:end)];



figure

hold on;
axis equal
%arena=imread('DAC_arena.png');
arena=imread('DAC_arena_ambiguous.png');
arena=flipdim(arena,1);
image(arena);



x=(x-7.5)/5*size(arena,2);
y=(y-7.5)/5*size(arena,1);


for ii=1:length(indexT)
	plotIndex=indexT(ii,1):indexT(ii,2);    	
    plot(x(plotIndex),y(plotIndex),'Color',[239, 205, 205]/255)
end
   
axis off



%plot(x(collisions),y(collisions),'.r');
%plot(x(targets),y(targets),'.g');

%Ncollisions = sum(diff(collisions)>0)
%Ntargets = sum(diff(targets)>0)