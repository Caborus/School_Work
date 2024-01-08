Program Gaussian_elimination
    implicit none

    Integer::n=2000 
    Integer i,j,k 
    Real(8),Allocatable :: a(:,:),b(:),x(:),c(:)
    Real(8) d
    Real start, end

    print *, "Enter size of the Matrix (100, 500, 1000, 1500, 2000): "
    read *, n

    Allocate(a(1:n,1:n+1),b(1:n),x(1:n),c(1:n))

    call RANDOM_NUMBER(a)
    call RANDOM_NUMBER(b)

    call cpu_time(start)

    do k=1,n-1,1
        do i=k+1,n,1
            if(a(k,k) /= 0) then
                a(n,i)=a(n,i)-a(i,k)/a(k,k)*a(n,k)
            else
                goto 100
            endif
            d=a(i,k)
            do j=1,n,1
                a(i,j)=a(i,j)-a(k,j)*(d/a(k,k))
            enddo
        enddo
    enddo

    do i=n,1,-1
        do j=1,n,1
            if(j /= i ) then
                c(i)=c(i)+a(i,j)*x(j)
            else
                cycle
            endif
        enddo
        x(i)=(a(n,i)-c(i))/a(i,i)
    enddo
    
    call cpu_time(end)
    print *, end-start

    100 stop

    end