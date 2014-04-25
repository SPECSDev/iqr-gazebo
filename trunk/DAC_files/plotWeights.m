skip = 2;
m=importdata('weights.dat',';',skip);

weights=m.data(:,2:end);

plot(smooth(sqrt(sum(weights.*weights,2)),10));

