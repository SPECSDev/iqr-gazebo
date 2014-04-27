function h = plotTrajectory(filename, varargin)
%PLOTTRAJECTORY   Creates a plot of the robot trajectories within the arena.
%   H = PLOTTRAJECTORY(FILENAME) returns the handle of the figure and plot
%   the drawing of the trajectories within the default arena
% 
%   H = PLOTTRAJECTORY(FILENAME,VARARGIN) returns the handle of the figure and plot
%   the drawing of the trajectories within the AMBIGUOUS arena
%
%   Example:
%      filename = '/home/user/Desktop/filename.dat';
%      h = plotTrajectory(filename)
%

%   This script is provided without ant warranty
%   $Date: 2014/04/26 $
%   Author: SPECS - UPF - Barcelona
skipheader = 2;
m = importdata(filename,';',skipheader);

x = m.data(:,2);
y = m.data(:,3);

indexT = find(diff(y)<-2);
indexT = [indexT(1:(end-1))+40,indexT(2:end)];

h = figure();
hold on;
axis equal
if (nargin>1)
    arena = imread('DAC_arena_ambiguous.png');
else
    arena = imread('DAC_arena.png');
end

arena = flipdim(arena,1);
image(arena);

x = (x-7.5)/5*size(arena,2);
y = (y-7.5)/5*size(arena,1);

for ii=1:length(indexT)
	plotIndex=indexT(ii,1):indexT(ii,2);    	
    plot(x(plotIndex),y(plotIndex),'Color',[239, 205, 205]/255, 'Linestyle', ':')
end
   
box off
grid off
axis off

end
