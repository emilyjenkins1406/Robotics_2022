function [] = savepdf(fighandle)
%SAVEPDF Create a nice pdf figure.
%
%   SAVEPDF('script') will run the MATLAB script named script.m - which
%   should generate a single figure - and generate a reasonable-looking PDF
%   file from it, called script.pdf
%
%   SAVEPDF('script','dir') will save the output in a directory 'dir'
%   (default is the current directory)

if ~exist('fighandle','var')
    % no output directory defined, output here
    fighandle=gcf;
end
fighandle;
fontsize = 22;          % map all fonts to this size
set(gca, 'DefaultTextInterpreter', 'latex')
set(gca, 'DefaultLegendInterpreter', 'latex')
set(gca, 'TickLabelInterpreter', 'latex')

outputsize = [22 12];   % output figure size [width height] in cm

fighandle.PaperUnits='centimeters';
fighandle.Position=[0 0 outputsize];
fighandle.PaperSize=outputsize;
fighandle.Visible='Off';

%disp(['Running ' scriptname]);
%func = str2func(scriptname);
%func();

axesHandle = gca;
set(axesHandle,'FontSize',fontsize)
figureHandle = gcf;
set(findall(figureHandle,'type','text'),'FontSize',fontsize)

fname = ['fig.pdf'];
disp(['Saving ' fname]);
print(figureHandle,fname,'-dpdf');

disp(['Saved ' fname]);
close(figureHandle);

end